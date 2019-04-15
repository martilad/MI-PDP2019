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
#include "mpi.h"

#include "solver.h"
#include <thread>
#include "../item.h"
#include "../logger.h"


class MPIParallel : public Solver {

protected:

    int nThreads;
    std::deque <Item> *queue;
    omp_lock_t writelock;

    // counter for try get best solution
    int cnt = 0;
    // theoretic best for problem if reach end the computing
    int theoreticBest;

    // number of generated problems on one CPU
    int generated;
    // number of generated task for slave process
    int mainGenerated;
    // rank of process
    int rank;
    // list of available slaves
    std::deque<int> slaves;

    // size of messages
    int messageSize;
    // tag for sending problems and solutions
    int tagProblem = 1;

    // message to send the problems and solutions
    int * messageSend;
    MPI_Status statusSend;
    MPI_Request requestSend;
    int sendRequestComplete = 0;

    // message to receive the problems and solutions
    int * messageRecv;
    MPI_Status statusRecv;
    MPI_Request requestRecv;
    int recvRequestComplete = 0;


    // value to broadcast the best solution for the cutting the problem
    int bestBCast;
    MPI_Request bestBCastRequest;
    int bCastRequestComplete = 0;

    // number of slaves
    int nSlaves;

    // best score
    int bestScore;

    // logger
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
    void recursionSingleThreadDFSnotMPI(solution * sol, cord co, int cnt);

public:
    /**
     * Constructor.
     *
     * @param nThreads number of threads in every process
     * @param mainGenerated number of generated problems on main process
     * @param generated generated problems on each process for threads
     * @param rank rank of the process
     * @param numberOfProcess number of processes
     * @param logger logger for logging (log are comment for the boost the time
     */
    MPIParallel(int nThreads, int mainGenerated, int generated, int rank, int numberOfProcess, LOGGER * logger);
    virtual ~MPIParallel();

    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();

};

#endif //MI_PDP2019_MPIPARALLEL_H
