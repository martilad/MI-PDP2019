//
// Created by martilad on 3/21/19.
//

#ifndef MI_PDP2019_MPIPARALLEL_H
#define MI_PDP2019_MPIPARALLEL_H


#include <deque>
#include <iostream>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include "solver.h"
#include "../item.h"
#include "mpi.h"


class MPIParallel : public Solver {

protected:

    int generated;
    int mainGenerated;
    int rank;
    std::deque<int> slaves;
    int * message;
    int message_size;
    int tag = 1;
    MPI_Status status;
    int nSlaves;

    /**
     * Generate instances to queue for threads.
     *
     * @param queue queue to push in
     * @param nRelaxation min number of items in queue, maximum is instCount + 16 in the worst case.
     */
    void generateInstBFS(std::deque<Item> * queue, int instCount);

    /**
     * Recursion for single thread solution, not copy recursion on one thread.
     *
     * @param sol pointer on solution
     * @param co coordinates to solve
     * @param cnt counter
     */
    void recursionSingleThreadDFS(solution * sol, cord co, int cnt);

public:

    MPIParallel(int nThreads, int mainGenerated, int generated, int rank, int numberOfProcess);
    virtual ~MPIParallel();

    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();

};

#endif //MI_PDP2019_MPIPARALLEL_H
