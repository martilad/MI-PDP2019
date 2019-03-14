//
// Created by ladislav on 3/14/19.
//
#include "../../headers/solvers/recursion.h"

Recursion::Recursion(): Solver() {}

Recursion::~Recursion() {}

void Recursion::solve(){
    this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
    this->workSolution.nEmptyBefore = 0;
    this->workSolution.nL3 = 0;
    this->workSolution.nL4 = 0;

    // init best solution
    this->bestSolution = this->workSolution;
    this->bestSolution.computePrice();

    // call the recursion
    this->recursion(this->workSolution.nextFree(-1, 0), 1);
}

void Recursion::recursion(cord co, int cnt){
    this->workSolution.computePrice();

    // if act solution is better than best solution -> replace
    if (this->workSolution.price > this->bestSolution.price) {
        this->bestSolution = this->workSolution;
    }

    //check if act price + possible price can beat the max
    this->workSolution.computeActPrice();
    if (this->workSolution.actPrice + helpers::eval_pol(this->workSolution.nEmptyAfter) <= this->bestSolution.price) {
        return;
    }

    if (co.x == -1 || co.y == -1 || co.y == this->m - 1) return;

    for (int id = l41; id < empty + 1; id++) {

        //add to map
        int ret = this->workSolution.addValueToMap(id, 0, cnt, co.x, co.y);
        if (ret == -1) {
            continue;
        }

        this->recursion(this->workSolution.nextFree(co.x, co.y), cnt + 1);

        ret = this->workSolution.addValueToMap(id, cnt, 0, co.x, co.y);
    }
}