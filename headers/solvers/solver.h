//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_SOLVER_H
#define MI_PDP2019_SOLVER_H

#include "../solution.h"

class Solver{

public:
    Solver();
    virtual ~Solver();
    /**
     * Solve the map with items in possibility.
     */
    virtual void solve()=0;

    /**
    * Getter for the best solution in map.
    * @return the best reach solution
    */
    solution getBest();

    /**
     * Load the problem from standard input.
     */
    void loadProblem(std::istream& in);

    /**
     * Check for successfully load the problem.
     * @return true if load the problem
     */
    bool isLoad();


protected:
    solution workSolution;
    solution bestSolution;
    int m;
    int n;
    int load;
};

#endif //MI_PDP2019_SOLVER_H
