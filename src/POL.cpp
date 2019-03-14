//
// Created by martilad on 3/14/19.
//

#include "../headers/POL.h"
#include "../headers/helpers.h"

/**
 * Getter for the best solution in map.
 * @return the best reach solution
 */
solution POL::getBest() {
    return this->bestSolution;
}

POL::POL(int nThreads, int depthTaskParallel) {
    this->nThreads = nThreads;
    this->depthTaskParallel = depthTaskParallel;
}

/**
 * Solution by recursion, init the recursion, copy the solution instance.
 */
void POL::solveRecursionCopy() {
    this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
    this->workSolution.nEmptyBefore = 0;
    this->workSolution.nL3 = 0;
    this->workSolution.nL4 = 0;

    // init best solution
    this->bestSolution = this->workSolution;
    this->bestSolution.computePrice();

# pragma omp parallel num_threads (nThreads)
    {
# pragma omp single
        this->solveRecursionCopy(this->workSolution, this->workSolution.nextFree(-1, 0), 1);
    }
}

/**
 * Recursion call for solve the instance.
 *
 * @param sol solution
 * @param co coordinates of position
 * @param cnt id of place item
 */
void POL::solveRecursionCopy(solution sol, cord co, int cnt) {

    sol.computePrice();

    // if act solution is better than best solution -> replace
    if (sol.price > this->bestSolution.price) {
#pragma omp critical
        if (sol.price > this->bestSolution.price)
            this->bestSolution = sol;
    }

    //check if act price + possible price can beat the max
    sol.computeActPrice();

    if (sol.actPrice + eval_pol(sol.nEmptyAfter) <= this->bestSolution.price) {
        return;
    }
    if (co.x == -1 || co.y == -1 || co.y == this->m - 1)return;

    for (int id = l41; id < empty + 1; id++) {

        // add new to map
        int ret = sol.addValueToMap(id, 0, cnt, co.x, co.y);
        if (ret == -1) {
            continue;
        } else if (ret == empty) {
            sol.nEmptyAfter -= 1;
            sol.nEmptyBefore += 1;
        } else if (ret >= l41 && ret <= l48) {
            sol.nL4 += 1;
            sol.nEmptyAfter -= 5;
        } else {
            sol.nL3 += 1;
            sol.nEmptyAfter -= 4;
        }

#pragma omp task if (cnt < this->depthTaskParallel )
        this->solveRecursionCopy(sol, sol.nextFree(co.x, co.y), cnt + 1);

        //remove from map to try new value
        ret = sol.addValueToMap(id, cnt, 0, co.x, co.y);
        if (ret == empty) {
            sol.nEmptyAfter += 1;
            sol.nEmptyBefore -= 1;
        } else if (ret >= l41 && ret <= l48) {
            sol.nL4 -= 1;
            sol.nEmptyAfter += 5;
        } else {
            sol.nL3 -= 1;
            sol.nEmptyAfter += 4;
        }
    }
}

/**
 * Recursion with no copy the solution, work wwith class variable.
 */
void POL::solveRecursionNotCopy() {
    this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
    this->workSolution.nEmptyBefore = 0;
    this->workSolution.nL3 = 0;
    this->workSolution.nL4 = 0;

    // init best solution
    this->bestSolution = this->workSolution;
    this->bestSolution.computePrice();

    // call the recursion
    this->solveRecursionNotCopy(this->workSolution.nextFree(-1, 0), 1);
}


/**
 * Recursion with no copy, only pass the coordinates.
 *
 * @param co coordinates of do position
 * @param cnt id of item on map
 */
void POL::solveRecursionNotCopy(cord co, int cnt) {

    this->workSolution.computePrice();

    // if act solution is better than best solution -> replace
    if (this->workSolution.price > this->bestSolution.price) {
        this->bestSolution = this->workSolution;
    }

    //check if act price + possible price can beat the max
    this->workSolution.computeActPrice();
    if (this->workSolution.actPrice + eval_pol(this->workSolution.nEmptyAfter) <= this->bestSolution.price) {
        return;
    }

    if (co.x == -1 || co.y == -1 || co.y == this->m - 1) return;

    for (int id = l41; id < empty + 1; id++) {

        //add to map
        int ret = this->workSolution.addValueToMap(id, 0, cnt, co.x, co.y);
        if (ret == -1) {
            continue;
        } else if (ret == empty) {
            this->workSolution.nEmptyAfter -= 1;
            this->workSolution.nEmptyBefore += 1;
        } else if (ret >= l41 && ret <= l48) {
            this->workSolution.nL4 += 1;
            this->workSolution.nEmptyAfter -= 5;
        } else {
            this->workSolution.nL3 += 1;
            this->workSolution.nEmptyAfter -= 4;
        }

        this->solveRecursionNotCopy(this->workSolution.nextFree(co.x, co.y), cnt + 1);

        ret = this->workSolution.addValueToMap(id, cnt, 0, co.x, co.y);
        if (ret == empty) {
            this->workSolution.nEmptyAfter += 1;
            this->workSolution.nEmptyBefore -= 1;
        } else if (ret >= l41 && ret <= l48) {
            this->workSolution.nL4 -= 1;
            this->workSolution.nEmptyAfter += 5;
        } else {
            this->workSolution.nL3 -= 1;
            this->workSolution.nEmptyAfter += 4;
        }
    }
}


/**
 * Solve the map with items in possibility.
 */
void POL::solveMap() {
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
        if (this->workSolution.actPrice + eval_pol(this->workSolution.nEmptyAfter) <= this->bestSolution.price) {
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

/**
 * Load the problem from standard input.
 */
void POL::loadProblem(std::istream &in) {
    // load arguments
    int k;
    in >> this->m >> this->n >> k;
    this->workSolution.m = this->m;
    this->workSolution.n = this->n;
    this->workSolution.k = k;
    this->workSolution.ground = std::vector < std::vector < int >> ();

    // init the ground
    for (int i = 0; i < m; i++) {
        std::vector<int> tmp = std::vector<int>();
        for (int j = 0; j < n; j++) {
            tmp.push_back(0);
        }
        this->workSolution.ground.push_back(tmp);
    }

    // load the forbidden fields
    for (int i = 0; i < k; i++) {
        int x, y;
        in >> x >> y;
        if (x >= 0 && x < n && y >= 0 && y < m) {
            this->workSolution.ground[y][x] = -1;
        }
    }
    this->load = true;
}

/**
 * Check for successfully load the problem.
 * @return true if load the problem
 */
bool POL::isLoad() {
    return this->load;
}

