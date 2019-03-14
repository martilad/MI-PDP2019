//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_POL_H
#define MI_PDP2019_POL_H

#include <vector>
#include <stack>
#include <queue>
#include "solution.h"
#include "stackItem.h"
#include "helpers.h"



/**
 * One solution class.
 */
class POL {
    solution workSolution;
    solution bestSolution;
    int m;
    int n;
    bool load;

public:
    int nThreads;
    int depthTaskParallel;
    /**
     * Getter for the best solution in map.
     * @return the best reach solution
     */
    solution getBest();

    POL (int nThreads, int depthTaskParallel);

    /**
     * Solution by recursion, init the recursion, copy the solution instance.
     */
    void solveRecursionCopy();

    /**
     * Recursion call for solve the instance.
     *
     * @param sol solution
     * @param co coordinates of position
     * @param cnt id of place item
     */
    void solveRecursionCopy(solution sol, cord co, int cnt);

    /**
     * Recursion with no copy the solution, work wwith class variable.
     */
    void solveRecursionNotCopy();

    /**
     * Recursion with no copy, only pass the coordinates.
     *
     * @param co coordinates of do position
     * @param cnt id of item on map
     */
    void solveRecursionNotCopy(cord co, int cnt);

    /**
     * Solve the map with items in possibility.
     */
    void solveMap();

    /**
     * Load the problem from standard input.
     */
    void loadProblem(std::istream& in);

    /**
     * Check for successfully load the problem.
     * @return true if load the problem
     */
    bool isLoad();
};

#endif //MI_PDP2019_POL_H
