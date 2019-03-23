//
// Created by martilad on 3/21/19.
//
#include "../../headers/solvers/MPIParallel.h"

MPIParallel::MPIParallel(int nThreads, int mainGenerated, int generated, int rank, int numberOfProcess): Solver() {
    this->rank = rank;
    this->generated = generated;
    this->mainGenerated = mainGenerated;
    this->nSlaves = numberOfProcess-1;
    if (rank == 0){
        for (int i = 1; i < numberOfProcess; i++){
            this->slaves.push_back(i);
        }
    }
#if defined(_OPENMP)
    omp_set_num_threads(nThreads);
#endif
}

MPIParallel::~MPIParallel() {}

void MPIParallel::solve(){

    this->message_size = 14+this->n*this->m;
    MPI_Bcast (&this->message_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    this->message = new int [this->message_size];

    if (this->rank == 0){

        this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
        this->workSolution.nEmptyBefore = 0;
        this->workSolution.nL3 = 0;
        this->workSolution.nL4 = 0;

        // init best solution
        this->bestSolution = this->workSolution;
        this->bestSolution.computePrice();

        std::deque<Item> * queue = new std::deque<Item>();
        queue->push_back(Item(this->workSolution, this->workSolution.nextFree(-1, 0), 1, this->bestSolution.price));

        this->generateInstBFS(queue, this->mainGenerated);//TODO:

        while (!queue->empty()) {

            Item tmp = queue->front();
            queue->pop_front();

            this->message = tmp.toMessage(this->message, this->rank);
            // sent to free slave process
            MPI_Send(this->message, this->message_size, MPI_INT, this->slaves.front(), this->tag, MPI_COMM_WORLD);
            this->slaves.pop_front();

            // if no free slave wait for some and get the solution
            if (this->slaves.empty()){
                MPI_Recv(this->message, this->message_size, MPI_INT, MPI_ANY_SOURCE, this->tag, MPI_COMM_WORLD, &this->status);
                this->slaves.push_back(this->message[13]);
                Item tmp = Item(this->message);
                tmp.sol.printSolution();
                std::cout<< "Get some reply from." << rank << std::endl;
            }
        }

        while (((int)this->slaves.size())!=this->nSlaves){
            //MPI_Recv(this->message, this->message_size, MPI_INT, MPI_ANY_SOURCE, this->tag, MPI_COMM_WORLD, &this->status);
            MPI_Recv(this->message, this->message_size, MPI_INT, MPI_ANY_SOURCE, this->tag, MPI_COMM_WORLD, &this->status);
            this->slaves.push_back(this->message[13]);
            Item tmp = Item(this->message);
            tmp.sol.printSolution();
            std::cout<< "Get some reply from." << rank << std::endl;
        }

        this->message[0] = -1;
        for (int i=0;i<this->nSlaves;i++) MPI_Send(this->message, this->message_size, MPI_INT, i+1, this->tag, MPI_COMM_WORLD);

        std::cout << "Master: Create some work!!" <<  std::endl;

    }else{


        bool end = false;
        while (!end) {
            MPI_Recv(this->message, this->message_size, MPI_INT, 0, this->tag, MPI_COMM_WORLD, &this->status);
            if (message[0] == -1) {
                end = true;
            } else {
                Item tmp = Item(this->message);
                tmp.sol.printSolution();

                //TODO: solve problem

                MPI_Send(this->message, this->message_size, MPI_INT, 0, this->tag, MPI_COMM_WORLD);
            }
        }
        std::cout << "Slave: I want the work!!" << std::endl;

    }

    /*this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
    this->workSolution.nEmptyBefore = 0;
    this->workSolution.nL3 = 0;
    this->workSolution.nL4 = 0;

    // init best solution
    this->bestSolution = this->workSolution;
    this->bestSolution.computePrice();

    std::deque<Item> * queue = new std::deque<Item>();
    queue->push_back(Item(this->workSolution, this->workSolution.nextFree(-1, 0), 1));

    this->generateInstBFS(queue, this->generated);

    // call the recursion
    unsigned int i=0;
#pragma omp parallel for private(i) schedule(dynamic)
    for (i = 0; i< queue->size();i++){
        this->recursionSingleThreadDFS(&((*queue)[i].sol), (*queue)[i].co, (*queue)[i].cnt);
    }*/
    delete[]message;
}

void MPIParallel::generateInstBFS(std::deque<Item> * queue, int instCount){

    while ((int)queue->size() < instCount){
        Item it = queue->front();
        queue->pop_front();

        for (int id = l41; id < empty + 1; id++) {
            //add to map
            Item tmp = it;
            int ret = tmp.sol.addValueToMap(id, 0, tmp.cnt, tmp.co.x, tmp.co.y);
            if (ret != -1){
                queue->push_back(Item(tmp.sol, tmp.sol.nextFree(tmp.co.x, tmp.co.y), tmp.cnt+1 ));//TODO: best
            }
        }
    }
}

void MPIParallel::recursionSingleThreadDFS(solution * sol, cord co, int cnt){

    sol->computePrice();

    // if act solution is better than best solution -> replace
    if (sol->price > this->bestSolution.price) {
#pragma omp critical
        if (sol->price > this->bestSolution.price)
            this->bestSolution = (*sol);
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

        this->recursionSingleThreadDFS(sol, sol->nextFree(co.x, co.y), cnt + 1);

        ret = sol->addValueToMap(id, cnt, 0, co.x, co.y);
    }
}
