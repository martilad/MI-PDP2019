//
// Created by martilad on 3/14/19.
//

#include "../../headers/solvers/taskParallel.h"

TaskParallel::TaskParallel(int nThreads, int depthTaskParallel): Solver() {
    this->nThreads = nThreads;
    this->depthTaskParallel = depthTaskParallel;
}

TaskParallel::~TaskParallel() {}

void TaskParallel::solve(){
    this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
    this->workSolution.nEmptyBefore = 0;
    this->workSolution.nL3 = 0;
    this->workSolution.nL4 = 0;

    // init best solution
    this->bestSolution = this->workSolution;
    this->bestSolution.computePrice();

    # pragma omp parallel num_threads (this->nThreads)
    {
        # pragma omp single
        this->taskParallel(this->workSolution, this->workSolution.nextFree(-1, 0), 1);
    }
}

void TaskParallel::taskParallel(solution sol, cord co, int cnt){
    sol.computePrice();

    // if act solution is better than best solution -> replace
    if (sol.price > this->bestSolution.price) {
        #pragma omp critical
        if (sol.price > this->bestSolution.price)
            this->bestSolution = sol;
    }

    //check if act price + possible price can beat the max
    sol.computeActPrice();

    if (sol.actPrice + helpers::eval_pol(sol.nEmptyAfter) <= this->bestSolution.price) {
        return;
    }
    if (co.x == -1 || co.y == -1 || co.y == this->m - 1)return;

    for (int id = l41; id < empty + 1; id++) {

        // add new to map
        int ret = sol.addValueToMap(id, 0, cnt, co.x, co.y);
        if (ret == -1) {
            continue;
        }

        #pragma omp task if (cnt-1 < this->depthTaskParallel )
        this->taskParallel(sol, sol.nextFree(co.x, co.y), cnt + 1);

        //remove from map to try new value
        ret = sol.addValueToMap(id, cnt, 0, co.x, co.y);
    }
}