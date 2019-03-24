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
#include "../logger.h"


class MPIParallel : public Solver {

protected:

    // number of generated problems on one CPU
    int generated;
    // number of generated task for slave process
    int mainGenerated;
    // rank of process
    int rank;
    // list of available slaves
    std::deque<int> slaves;
    // message to send the problems and solutions
    int * message;
    // size of message
    int message_size;
    int tag = 1;
    MPI_Status status;
    // number of slaves
    int nSlaves;
    // best score
    int bestScore;

    LOGGER * logger;

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

    MPIParallel(int nThreads, int mainGenerated, int generated, int rank, int numberOfProcess, LOGGER * logger);
    virtual ~MPIParallel();

    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();

};

#endif //MI_PDP2019_MPIPARALLEL_H
