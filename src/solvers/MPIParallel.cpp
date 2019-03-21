//
// Created by martilad on 3/21/19.
//
#include "../../headers/solvers/MPIParallel.h"

MPIParallel::MPIParallel(int nThreads, int generated, int rank): Solver() {
    this->rank = rank;
    this->generated = generated;
#if defined(_OPENMP)
    omp_set_num_threads(nThreads);
#endif
}

MPIParallel::~MPIParallel() {}

void MPIParallel::solve(){
    if (this->rank == 0){
        std::cout << "Master" << std::endl;
        this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
        this->workSolution.nEmptyBefore = 0;
        this->workSolution.nL3 = 0;
        this->workSolution.nL4 = 0;

        // init best solution
        this->bestSolution = this->workSolution;
        this->bestSolution.computePrice();
        //MPI_Recv();
        return;
    }else{
        std::cout << "Slave" << std::endl;
        return;
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
                queue->push_back(Item(tmp.sol, tmp.sol.nextFree(tmp.co.x, tmp.co.y), tmp.cnt+1));
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
