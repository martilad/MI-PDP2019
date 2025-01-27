//
// Created by martilad on 3/14/19.
//
#include <iostream>
#include <fstream>
#include <string.h>

#if defined(_OPENMP)
#include <omp.h>
#endif

#include "mpi.h"
#include "../headers/solution.h"
#include "../headers/solvers/solver.h"
#include "../headers/solvers/recursion.h"
#include "../headers/solvers/dataParallel.h"
#include "../headers/solvers/taskParallel.h"
#include "../headers/solvers/MPIParallel.h"
#include "../headers/helpers.h"
#include "../headers/logger.h"

int main(int argc, char *argv[]) {
    char *myFile = nullptr;
    int run = 0;
    int nT = -1;
    int nN = -1;
    int nNP = -1;
    int my_rank;
    int p;
    bool go = true;

    omp_set_nested(1);

    helpers::load_arguments(&myFile, &run, &nT, &nN, &nNP, argc, argv);

    /* start up MPI */
    MPI_Init(&argc, &argv);

    /* find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //LOGGER *logger = new LOGGER(my_rank, "procLog", "./", ERROR);
    //LOGGER *logger = new LOGGER(my_rank, "procLog", "./", INFO);
    LOGGER * logger = nullptr;

    Solver *problem;
    switch (run) {
        case 1:
            if (nT < 1 || nN < 0) {
                std::cout << "Specific real number of thread (-nT) or positive number of generated tasks (-nN)."
                          << std::endl;
                exit(1);
            }
            go = false;
            problem = new DataParallel(nT, nN);
            break;
        case 2:
            go = false;
            problem = new Recursion();
            break;
        case 3:
            if (nT < 1 || nN < 0) {
                std::cout << "Specific real number of thread (-nT) or positive number of generated tasks (-nN)."
                          << std::endl;
                exit(1);
            }
            go = false;
            problem = new TaskParallel(nT, nN);
            break;
        case 4:
            if (nT < 1 || nN < 0 || nNP < 0) {
                std::cout << "Specific real number of thread on one slave (-nT) ";
                std::cout << "and positive number of generated tasks in one level (-nN).";
                std::cout << "and specific number of task distributed between the processes. (-nNP)" << std::endl;
                exit(1);
            }
            go = true;
            problem = new MPIParallel(nT, nNP, nN, my_rank, p, logger);
            break;
        default:
            std::cout << "Please specific the algorithm: \"-dp\" data parallel solution;";
            std::cout << ", \"-r\" recursion with share memory (the fastest single thread implementation), ";
            std::cout << "\"-tp\" task parallelism recursion with copying or" << std::endl;
            std::cout << "\"-mpi\" for solve using MPI with distributed memory." << std::endl;
            exit(1);
    }
    //logger->info("Number of process: " + std::to_string(p));
    //logger->info("Number of threads in process: " + std::to_string(nT));
    //logger->info("Number of generated problems for process: " + std::to_string(nN));
    //logger->info("Number of generated problems for threads in process: " + std::to_string(nNP));
    if (my_rank == 0) {
        // load the problem
        if (myFile) {
            std::ifstream file;
            file.open(myFile);
            if (file.is_open()) {
                problem->loadProblem(file);
                file.close();
            }
        } else {
            std::cout << "insert problem... (or use -f FILE for file)" << std::endl;
            problem->loadProblem(std::cin);
        }
    }
    if (go && my_rank != 0) {
        //clock_t begin = clock();


#if defined(_OPENMP)
        //double beginR = omp_get_wtime();
#endif

        problem->solve();
        //clock_t end = clock();


#if defined(_OPENMP)
        //double endR = omp_get_wtime();
#endif

        //double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        //logger->info("Solution proc time: " + std::to_string(elapsed_secs) + " s.");

#if defined(_OPENMP)
        //logger->info("Solution real time (if parralel): "+ std::to_string(endR - beginR) + " s.");
#endif


        delete problem;
    }

    if (!go && my_rank != 0) {
        delete problem;
    }

    int t1 = MPI_Wtime();

    if (my_rank == 0) {
        clock_t begin = clock();


#if defined(_OPENMP)
        double beginR = omp_get_wtime();
#endif

        problem->solve();
        clock_t end = clock();


#if defined(_OPENMP)
        double endR = omp_get_wtime();
#endif
        if (my_rank == 0) {
            solution best = problem->getBest();
            best.printSolution();
        }
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "Solution proc time: " << elapsed_secs << " s." << std::endl;

#if defined(_OPENMP)
        std::cout << "Solution real time (if parralel): "<< endR - beginR << " s." << std::endl;
#endif
        delete problem;
    }

    //logger->info("Elapsed time is: " + std::to_string(MPI_Wtime() - t1) + " s.");
    std::cout << "Elapsed time on worker " << my_rank <<" is: " << MPI_Wtime() - t1 << " s." << std::endl;
    //logger->info("Elapsed time is: " + std::to_string(MPI_Wtime() - t1) + " s.");
    //logger->info("Calling MPI_Finalize...");
    MPI_Finalize();
    //logger->info("MPI_Finalize completed");
}