//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_TASKPARALLEL_H
#define MI_PDP2019_TASKPARALLEL_H

#include "solver.h"

class TaskParallel : public Solver {

protected:

    int nThreads;
    int depthTaskParallel;

    /**
     * Recursion with no copy, only pass the coordinates.
     *
     * @param co coordinates of do position
     * @param cnt id of item on map
     */
    void taskParallel(solution sol, cord co, int cnt);

public:

    TaskParallel(int nThreads, int depthTaskParallel);
    virtual ~TaskParallel();

    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();



};

#endif //MI_PDP2019_TASKPARALLEL_H
