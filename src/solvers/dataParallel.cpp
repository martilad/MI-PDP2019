//
// Created by martilad on 3/14/19.
//

#include "../../headers/solvers/dataParallel.h"

DataParallel::DataParallel(int nThreads, int generated): Solver() {
    this->nThreads = nThreads;
    this->generated = generated;
}

DataParallel::~DataParallel() {}

void generateInstBFS(){

}

void solveOneInst(Item it){

}

void recursionSingleThreadDFS(solution * sol, cord co, int cnt){

}



void DataParallel::solve(){
    // iteration stack for operations
    std::stack <stackItem> iterationStack;
    this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
    this->workSolution.nEmptyBefore = 0;
    this->workSolution.nL3 = 0;
    this->workSolution.nL4 = 0;

    // init best solution
    this->bestSolution = this->workSolution;
    this->bestSolution.computePrice();

    // add first possible place for add some item
    iterationStack.push(stackItem(this->workSolution.nextFree(-1, 0), no, 1));

    while (iterationStack.size() > 0) {
        stackItem tmp = iterationStack.top();
        iterationStack.pop();
        // if empty go back to next (back) position with some. (after place empty there is no next possibility there)
        if (tmp.id == empty) {
            this->workSolution.nEmptyAfter += 1;
            this->workSolution.nEmptyBefore -= 1;
            continue;
        }

        // if there some item remove it, recount counts of items
        if (tmp.id > no) {
            this->workSolution.addValueToMap(tmp.id, tmp.cnt, 0, tmp.coordinates.x, tmp.coordinates.y);
            if (tmp.id >= l41 && tmp.id <= l48) {
                this->workSolution.nL4 -= 1;
                this->workSolution.nEmptyAfter += 5;
            }
            if (tmp.id >= l31 && tmp.id <= l38) {
                this->workSolution.nL3 -= 1;
                this->workSolution.nEmptyAfter += 4;
            }
        }

        // add some next item on position, add item count and empty places count
        while (1) {
            int ret = this->workSolution.addValueToMap(++tmp.id, 0, tmp.cnt, tmp.coordinates.x, tmp.coordinates.y);
            if (tmp.id == empty || ret != -1) {
                if (tmp.id >= l41 && tmp.id <= l48) {
                    this->workSolution.nL4 += 1;
                    this->workSolution.nEmptyAfter -= 5;
                }
                if (tmp.id >= l31 && tmp.id <= l38) {
                    this->workSolution.nL3 += 1;
                    this->workSolution.nEmptyAfter -= 4;
                }
                if (tmp.id == empty) {
                    this->workSolution.nEmptyAfter -= 1;
                    this->workSolution.nEmptyBefore += 1;
                }
                break;
            }
        }

        // add placed item to iteration stack
        iterationStack.push(stackItem(cord(tmp.coordinates.x, tmp.coordinates.y), tmp.id, tmp.cnt));

        // check act price
        this->workSolution.computePrice();

        // if act solution is better than best solution -> replace
        if (this->workSolution.price > this->bestSolution.price) {
            this->bestSolution = this->workSolution;
        }
        // check if act price + possible price can beat the max
        this->workSolution.computeActPrice();
        if (this->workSolution.actPrice + helpers::eval_pol(this->workSolution.nEmptyAfter) <= this->bestSolution.price) {
            continue;
        }

        // cant put some in the last line
        if (tmp.coordinates.y != this->m - 1) {
            if (tmp.id < empty) tmp.cnt++;
            cord next = this->workSolution.nextFree(tmp.coordinates.x, tmp.coordinates.y);

            if (next.x != -1 && next.y != -1) {
                // add new free field to check possibility of this field
                iterationStack.push(stackItem(next, no, tmp.cnt));
            }
        }
    }
}
