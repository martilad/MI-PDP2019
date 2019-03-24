//
// Created by martilad on 3/21/19.
//
#include "../../headers/solvers/MPIParallel.h"

MPIParallel::MPIParallel(int nThreads, int mainGenerated, int generated, int rank, int numberOfProcess, LOGGER * logger)
        : Solver() {
    this->rank = rank;
    this->generated = generated;
    this->mainGenerated = mainGenerated;
    this->nSlaves = numberOfProcess - 1;
    this->logger = logger;
    if (rank == 0) {
        for (int i = 1; i < numberOfProcess; i++) {
            this->slaves.push_back(i);
        }
    }
#if defined(_OPENMP)
    omp_set_num_threads(nThreads);
#endif
}

MPIParallel::~MPIParallel() {}

void MPIParallel::solve() {

    this->message_size = 14 + this->n * this->m;
    MPI_Bcast(&this->message_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    this->message = new int[this->message_size];

    if (this->rank == 0) {

        // Init the first solution
        this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
        this->workSolution.nEmptyBefore = 0;
        this->workSolution.nL3 = 0;
        this->workSolution.nL4 = 0;

        // init best solution
        this->bestSolution = this->workSolution;
        this->bestSolution.computePrice();

        // init queue of problems with first start problem
        std::deque <Item> *queue = new std::deque<Item>();
        queue->push_back(Item(this->workSolution, this->workSolution.nextFree(-1, 0), 1, this->bestSolution.price));

        this->logger->info("Generating inst for proc: " + std::to_string(this->mainGenerated));
        // generate some inst for process
        this->generateInstBFS(queue, this->mainGenerated);
        this->logger->info("Generated inst: " + std::to_string(queue->size()));
        while (!queue->empty()) {

            // pop problem from queue to send to some worker
            Item tmp = queue->front();
            //Item tmp = queue->back();
            //queue->pop_back();
            queue->pop_front();

            // create message from Item and set the best score
            tmp.bestScore = this->bestSolution.price;
            this->message = tmp.toMessage(this->message, this->rank);
            // sent to free slave process
            this->logger->info("Send message to slave: " + std::to_string(this->slaves.front()));
            MPI_Send(this->message, this->message_size, MPI_INT, this->slaves.front(), this->tag, MPI_COMM_WORLD);
            this->logger->info("Send.");
            this->slaves.pop_front();

            // if no free slave wait for some and get the solution
            if (this->slaves.empty()) {
                this->logger->info("Wait for message from slave.");
                MPI_Recv(this->message, this->message_size, MPI_INT, MPI_ANY_SOURCE, this->tag, MPI_COMM_WORLD,
                         &this->status);
                this->logger->info("Get message from: " + std::to_string(this->message[13]) + ". Best done: " + std::to_string(this->message[12]));
                // save free slave worker
                this->slaves.push_back(this->message[13]);
                // check get message for the best solution
                if (this->bestSolution.price < this->message[12]) {
                    Item tmp = Item(this->message);
                    this->bestSolution = tmp.sol;
                }
            }
        }

        // get all slaves to idle
        while (((int) this->slaves.size()) != this->nSlaves) {
            this->logger->info("Wait for message from slave.");
            MPI_Recv(this->message, this->message_size, MPI_INT, MPI_ANY_SOURCE, this->tag, MPI_COMM_WORLD,
                     &this->status);
            this->logger->info("Get message from: " + std::to_string(this->message[13]) + ". Best done: " + std::to_string(this->message[12]));
            // save free slave worker
            this->slaves.push_back(this->message[13]);
            // check get message for the best solution
            if (this->bestSolution.price < this->message[12]) {
                Item tmp = Item(this->message);
                this->bestSolution = tmp.sol;
            }
        }

        // set end flag for workers
        this->message[0] = -1;
        for (int i = 0; i < this->nSlaves; i++) {
            this->logger->info("Send END message to slave: " + std::to_string(i+1));
            MPI_Send(this->message, this->message_size, MPI_INT, i + 1, this->tag, MPI_COMM_WORLD);
            this->logger->info("Send.");
        }


    } else {
        bool end = false;
        while (!end) {
            // get message from master with work or end
            MPI_Recv(this->message, this->message_size, MPI_INT, 0, this->tag, MPI_COMM_WORLD, &this->status);

            if (message[0] == -1) {
                // end go home
                this->logger->info("Get END message from master.");
                end = true;
            } else {

                // there was some work do it!!
                Item tmp = Item(this->message);
                this->logger->info("Get message from master with max score: " + std::to_string(tmp.bestScore));
                this->bestSolution = tmp.sol;
                this->bestScore = tmp.bestScore;
                std::deque <Item> *slave_queue = new std::deque<Item>();
                slave_queue->push_back(tmp);

                this->logger->info("Generating inst for threads on proc: " + std::to_string(this->generated));
                // generated some problems on thread
                this->generateInstBFS(slave_queue, this->generated);
                this->logger->info("Generated inst: " + std::to_string(slave_queue->size()));


                unsigned int i = 0;
#pragma omp parallel for private(i) schedule(dynamic)
                for (i = 0; i < slave_queue->size(); i++) {
                    this->recursionSingleThreadDFS(&((*slave_queue)[i].sol), (*slave_queue)[i].co,
                                                   (*slave_queue)[i].cnt);
                }
                this->logger->info("The problem solved.");
                // send back the best
                Item it = Item(this->bestSolution, cord(-1, -1), -1);
                it.bestScore = this->bestScore;
                it.toMessage(this->message, this->rank);
                this->logger->info("Send the results to master.");
                MPI_Send(this->message, this->message_size, MPI_INT, 0, this->tag, MPI_COMM_WORLD);
                this->logger->info("Send.");
            }
        }
    }
    delete[]message;
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

    // if act solution is better than best solution -> replace
    if (sol->price > this->bestScore) {
#pragma omp critical
        if (sol->price > this->bestScore) {
            this->bestSolution = (*sol);
            this->bestScore = (*sol).price;
        }
    }

    sol->computeActPrice();

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
