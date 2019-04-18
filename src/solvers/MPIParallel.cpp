//
// Created by martilad on 3/21/19.
//
#include "../../headers/solvers/MPIParallel.h"

MPIParallel::MPIParallel(int nThreads, int mainGenerated, int generated, int rank, int numberOfProcess, LOGGER *logger)
        : Solver() {
    omp_init_lock(&this->writelock);
    // the number of process
    this->rank = rank;
    this->nThreads = nThreads;
    this->generated = generated;
    this->mainGenerated = mainGenerated;
    this->nSlaves = numberOfProcess - 1;
    this->logger = logger;
    // if master create queue of slaves for sending work
    if (rank == 0) {
        for (int i = 1; i < numberOfProcess; i++) {
            this->slaves.push_back(i);
        }
    }
#if defined(_OPENMP)
    omp_set_num_threads(nThreads);
#endif
}

MPIParallel::~MPIParallel() {
    omp_destroy_lock(&this->writelock);
}

void MPIParallel::solve() {
    // compute size of message to allocate
    this->messageSize = 14 + this->n * this->m;
    // broadcast the size to slaves - only master load the data file
    MPI_Bcast(&this->messageSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // allocate the message buffers
    this->messageSend = new int[this->messageSize];
    this->messageRecv = new int[this->messageSize];

    omp_set_lock(&this->writelock);
    // master process

    if (this->rank == 0) {
        #pragma omp parallel num_threads (2)
        {
            if (omp_get_thread_num() == 0) {
                bool send = false;
                // Init the first solution
                this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
                this->workSolution.nEmptyBefore = 0;
                this->workSolution.nL3 = 0;
                this->workSolution.nL4 = 0;

                // compute theoretical best for end the computing
                //this->logger->info("Theoretic best: " + std::to_string(helpers::eval_pol(this->m*this->n-this->workSolution.k)));
                this->theoreticBest = helpers::eval_pol(this->m * this->n - this->workSolution.k);

                // init best solution
                this->bestSolution = this->workSolution;
                this->bestSolution.computePrice();

                // init queue of problems with first start problem
                this->queue = new std::deque<Item>();
                this->queue->push_back(Item(this->workSolution, this->workSolution.nextFree(-1, 0), 1, this->bestSolution.price));

                //this->logger->info("Generating inst for proc: " + std::to_string(this->mainGenerated));
                // generate some inst for process
                this->generateInstBFS(this->queue, this->mainGenerated);
                omp_unset_lock(&this->writelock);
                //this->logger->info("Generated inst: " + std::to_string(queue->size()));
                // for all generated
                while (true) {
                    Item tmp;
                    bool off = false;

                    #pragma omp critical
                    {
                        if (this->queue->empty()) {
                            off = true;
                        } else {
                            // pop problem from queue to send to some worker
                            tmp = this->queue->front();
                            //Item tmp = queue->back();
                            //queue->pop_back();
                            this->queue->pop_front();
                        }
                    }
                    if (off)break;
                    // wait for free buffer need send message, send flag for start test
                    if (send && !this->sendRequestComplete) {
                        MPI_Wait(&this->requestSend, &this->statusSend);
                    }
                    // create message from Item and set the best score

                    #pragma omp critical
                    {
                        tmp.bestScore = this->bestSolution.price;
                        tmp.toMessage(this->messageSend, this->rank);
                    }

                    //this->logger->info("Send message to slave: " + std::to_string(this->slaves.front()));
                    // sent to free slave process
                    MPI_Isend(this->messageSend, this->messageSize, MPI_INT, this->slaves.front(), this->tagProblem,
                              MPI_COMM_WORLD, &this->requestSend);
                    send = true;
                    //this->logger->info("Send.");
                    this->slaves.pop_front();

                    // test if send
                    MPI_Test(&this->requestSend, &this->sendRequestComplete, &this->statusSend);

                    // if no free slave wait for some and get the solution
                    if (this->slaves.empty()) {
                        //this->logger->info("Wait for message from slave.");

                        MPI_Irecv(this->messageRecv, this->messageSize, MPI_INT, MPI_ANY_SOURCE, this->tagProblem,
                                  MPI_COMM_WORLD, &this->requestRecv);
                        MPI_Test(&this->requestRecv, &this->recvRequestComplete, &this->statusRecv);
                        if (!this->recvRequestComplete) {
                            MPI_Wait(&this->requestRecv, &this->statusRecv);
                        }
                        //this->logger->info("Get message from: " + std::to_string(this->messageRecv[13]) + ". Best done: " +
                        //                  std::to_string(this->messageRecv[12]));
                        // save free slave worker
                        this->slaves.push_back(this->messageRecv[13]);
                        // check get message for the best solution
                        if (this->bestSolution.price < this->messageRecv[12]) {
                            #pragma omp critical
                            {
                                if (this->bestSolution.price < this->messageRecv[12]) {
                                    Item tmp = Item(this->messageRecv);

                                    this->bestBCast = tmp.sol.price;

                                    //this->logger->info("Broadcast the best solution: " + std::to_string(this->bestBCast));

                                    // broadcast new best solution to workers
                                    MPI_Ibcast(&this->bestBCast, 1, MPI_INT, 0, MPI_COMM_WORLD,
                                               &this->bestBCastRequest);

                                    //this->logger->info("Broadcasted.");
                                    this->bestSolution = tmp.sol;
                                }
                            }
                        }
                    }
                    // if reach the best -> end
                    if (this->bestSolution.price == this->theoreticBest) {
                        //this->logger->info("Reach the theoretic maximum, send end to slaves.");
                        break;
                    }
                }

                // get all slaves to idle for send the end -> get the best solutions
                while (((int) this->slaves.size()) != this->nSlaves) {
                    //this->logger->info("Wait for message from slave.");

                    MPI_Irecv(this->messageRecv, this->messageSize, MPI_INT, MPI_ANY_SOURCE, this->tagProblem,
                              MPI_COMM_WORLD, &this->requestRecv);

                    MPI_Test(&this->requestRecv, &this->recvRequestComplete, &this->statusRecv);

                    if (!this->recvRequestComplete) {
                        MPI_Wait(&this->requestRecv, &this->statusRecv);
                    }

                    //this->logger->info("Get message from: " + std::to_string(this->messageRecv[13]) + ". Best done: " +
                    //                   std::to_string(this->messageRecv[12]));
                    // save free slave worker
                    this->slaves.push_back(this->messageRecv[13]);
                    // check get message for the best solution

                    if (this->bestSolution.price < this->messageRecv[12]) {
                    #pragma omp critical
                        {
                            if (this->bestSolution.price < this->messageRecv[12]) {
                                Item tmp = Item(this->messageRecv);
                                this->bestBCast = tmp.sol.price;

                                //this->logger->info("Broadcast the best solution: " + std::to_string(this->bestBCast));

                                // broadcast new best solution to workers
                                MPI_Ibcast(&this->bestBCast, 1, MPI_INT, 0, MPI_COMM_WORLD, &this->bestBCastRequest);

                                //this->logger->info("Broadcasted.");

                                this->bestSolution = tmp.sol;
                            }
                        }
                    }

                }
                // set end flag for workers
                this->messageSend[0] = -1;
                for (int i = 0; i < this->nSlaves; i++) {
                    //this->logger->info("Send END message to slave: " + std::to_string(i + 1));
                    MPI_Send(this->messageSend, this->messageSize, MPI_INT, i + 1, this->tagProblem, MPI_COMM_WORLD);
                    //this->logger->info("Send.");
                }
            } else {
                //drop from queue and solving
                omp_set_lock(&this->writelock);
                if (this->nThreads > 1) {

                    Item tmp;
                    bool off = false;
                    while(true){
                        #pragma omp critical
                        {
                            if (this->queue->empty()) {
                                off = true;
                            } else {
                                //pop problem from queue to send to some worker
                                tmp = this->queue->front();
                                //Item tmp = queue->back();
                                //queue->pop_back();
                                this->queue->pop_front();
                            }
                        }
                        if (off)break;

                        // queue for problems in slave
                        std::deque <Item> *slave_queue = new std::deque<Item>();
                        slave_queue->push_back(tmp);
                        //this->logger->info("Generating inst for threads on proc: " + std::to_string(this->generated));
                        // generated some problems on thread
                        this->generateInstBFS(slave_queue, this->generated);
                        //this->logger->info("Generated inst: " + std::to_string(slave_queue->size()));

                        // start threads in slave
                        unsigned int i = 0;
                        #pragma omp parallel for private(i) schedule(dynamic) num_threads (this->nThreads-1)
                        for (i = 0; i < slave_queue->size(); i++) {
                            this->recursionSingleThreadDFSnotMPI(&((*slave_queue)[i].sol), (*slave_queue)[i].co,
                                                           (*slave_queue)[i].cnt);
                        }
                    }
                }
            }
        }
    } else {
        bool end = false;
        while (!end) {
            // get message from master with work or end
            MPI_Recv(this->messageRecv, this->messageSize, MPI_INT, 0, this->tagProblem, MPI_COMM_WORLD,
                     &this->statusRecv);

            if (messageRecv[0] == -1) {
                // end go home
                //this->logger->info("Get END message from master.");
                end = true;
            } else {
                cnt = 0;
                // there was some work do it!!
                Item tmp = Item(this->messageRecv);
                //this->logger->info("Get message from master with max score: " + std::to_string(tmp.bestScore));
                this->bestSolution = tmp.sol;
                this->bestScore = tmp.bestScore;
                // queue for problems in slave
                std::deque <Item> *slave_queue = new std::deque<Item>();
                slave_queue->push_back(tmp);

                //this->logger->info("Generating inst for threads on proc: " + std::to_string(this->generated));
                // generated some problems on thread
                this->generateInstBFS(slave_queue, this->generated);
                //this->logger->info("Generated inst: " + std::to_string(slave_queue->size()));


                // start threads in slave
                unsigned int i = 0;
                #pragma omp parallel for private(i) schedule(dynamic)
                for (i = 0; i < slave_queue->size(); i++) {
                    this->recursionSingleThreadDFS(&((*slave_queue)[i].sol), (*slave_queue)[i].co, (*slave_queue)[i].cnt);
                }
                //this->logger->info("The problem solved.");
                // send back the best
                Item it = Item(this->bestSolution, cord(-1, -1), -1);
                it.bestScore = this->bestScore;
                it.toMessage(this->messageSend, this->rank);
                // send results back
                //this->logger->info("Send the results to master.");

                MPI_Isend(this->messageSend, this->messageSize, MPI_INT, 0, this->tagProblem, MPI_COMM_WORLD,
                          &this->requestSend);

                //MPI_Send(this->message_send, this->message_size, MPI_INT, 0, this->tag_problem, MPI_COMM_WORLD);
                MPI_Test(&this->requestSend, &this->sendRequestComplete, &this->statusSend);
                if (!this->sendRequestComplete) {
                    MPI_Wait(&this->requestSend, &this->statusSend);
                }
                //this->logger->info("Send." + std::to_string(this->cnt));
            }
        }
    }
    // free messages buffers
    delete[]
            messageSend;
    delete[]
            messageRecv;
}

void MPIParallel::generateInstBFS(std::deque <Item> *queue, int instCount) {

    while ((int) queue->size() < instCount) {
        Item it = queue->front();
        queue->pop_front();

        for (int id = l41; id < empty + 1; id++) {
            //add to map
            Item tmp = it;
            int ret = tmp.sol.addValueToMap(id, 0, tmp.cnt, tmp.co.x, tmp.co.y);
            if (ret != -1) {
                queue->push_back(Item(tmp.sol, tmp.sol.nextFree(tmp.co.x, tmp.co.y), tmp.cnt + 1, it.bestScore));
            }
        }
    }
}

void MPIParallel::recursionSingleThreadDFS(solution *sol, cord co, int cnt) {

    sol->computePrice();

    // counter for check the broad casted best solution
    #pragma omp critical
    this->cnt++;
    if (this->cnt % 10000 == 0) {
        #pragma omp critical
        {
            //this->logger->info("Try to recv best solution");

            MPI_Ibcast(&this->bestBCast, 1, MPI_INT, 0, MPI_COMM_WORLD, &this->bestBCastRequest);

            MPI_Test(&this->bestBCastRequest, &this->bCastRequestComplete, MPI_STATUS_IGNORE);

            //this->logger->info("Recv bcast best solution: " + std::to_string(this->bestBCast));
            // if the best -> set new
            if (!this->bCastRequestComplete) {
                if (this->bestBCast > this->bestScore) {
                    this->bestScore = this->bestBCast;
                }
            }
        }
    }


    // if act solution is better than best solution -> replace
    if (sol->price > this->bestScore) {
        #pragma omp critical
        if (sol->price > this->bestScore) {
            this->bestSolution = (*sol);
            this->bestScore = (*sol).price;
        }
    }

    sol->computeActPrice();

    // cut the solution
    if (sol->actPrice + helpers::eval_pol(sol->nEmptyAfter) <= this->bestScore) {
        return;
    }

    if (co.x == -1 || co.y == -1 || co.y == this->m - 1) return;

    for (int id = l41; id < empty + 1; id++) {

        //add to map
        int ret = sol->addValueToMap(id, 0, cnt, co.x, co.y);
        if (ret == -1) {
            continue;
        }

        this->recursionSingleThreadDFS(sol, sol->nextFree(co.x, co.y), cnt + 1);

        ret = sol->addValueToMap(id, cnt, 0, co.x, co.y);
    }
}


void MPIParallel::recursionSingleThreadDFSnotMPI(solution * sol, cord co, int cnt){
    sol->computePrice();
    // if act solution is better than best solution -> replace
    if (sol->price > this->bestSolution.price) {
        #pragma omp critical
        if (sol->price > this->bestSolution.price){
            this->bestSolution = (*sol);
            this->bestScore = (*sol).price;
        }
    }
    sol->computeActPrice();

    if (sol->actPrice + helpers::eval_pol(sol->nEmptyAfter) <= this->bestSolution.price) {
        return;
    }

    if (co.x == -1 || co.y == -1 || co.y == this->m - 1) return;

    for (int id = l41; id < empty + 1; id++) {

        //add to map
        int ret = sol->addValueToMap(id, 0, cnt, co.x, co.y);
        if (ret == -1) {
            continue;
        }

        this->recursionSingleThreadDFSnotMPI(sol, sol->nextFree(co.x, co.y), cnt + 1);

        ret = sol->addValueToMap(id, cnt, 0, co.x, co.y);
    }
}