//
// Created by martilad on 3/14/19.
//
#include "../../headers/solvers/solver.h"


Solver::Solver() {}

Solver::~Solver() {}

solution Solver::getBest(){
    return this->bestSolution;
}

/**
 * Load the problem from standard input.
 */
void Solver::loadProblem(std::istream& in){
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
bool Solver::isLoad(){
    return this->load;
}