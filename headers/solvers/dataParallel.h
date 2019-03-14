//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_DATAPARALLEL_H
#define MI_PDP2019_DATAPARALLEL_H

#include <deque>
#include "solver.h"
#include "../item.h"


class DataParallel : public Solver {

protected:

    int nThreads;
    int generated;

    /**
     * Generate instances to queue for threads.
     *
     * @param queue queue to push in
     * @param nRelaxation min number of items in queue, maximum is instCount + 16 in the worst case.
     */
    void generateInstBFS(std::deque<Item> * queue, unsigned int instCount);

    /**
     * Recursion for single thread solution, not copy recursion on one thread.
     *
     * @param sol pointer on solution
     * @param co coordinates to solve
     * @param cnt counter
     */
    void recursionSingleThreadDFS(solution * sol, cord co, int cnt);

public:

    DataParallel(int nThreads, int generated);
    virtual ~DataParallel();

    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();





};

#endif //MI_PDP2019_DATAPARALLEL_H
