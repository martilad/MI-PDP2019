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

int main(int argc, char* argv[]) {
    char *myFile = nullptr;
    int run = 0;
    int nT = -1;
    int nN = -1;
    int nNP = -1;
    int my_rank;
    int p;
    bool go = true;

    helpers::load_arguments(&myFile, &run, &nT, &nN, &nNP, argc, argv);

    /* start up MPI */
    MPI_Init( &argc, &argv );

    /* find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    Solver * problem;
    switch (run){
        case 1:
            if (nT < 1 || nN < 0){
                std::cout << "Specific real number of thread (-nT) or positive number of generated tasks (-nIN)." << std::endl;
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
            if (nT < 1 || nN < 0){
                std::cout << "Specific real number of thread (-nT) or positive number of generated tasks (-nN)." << std::endl;
                exit(1);
            }
            go = false;
            problem = new TaskParallel(nT, nN);
            break;
        case 4:
            problem = new MPIParallel(nT, nN, my_rank);
            break;
        default:
            std::cout << "Please specific the algorithm: \"-dp\" data parallel solution;";
            std::cout << ", \"-r\" recursion with share memory (the fastest single thread implementation), ";
            std::cout << "\"-tp\" task parallelism recursion with copying." << std::endl;
            exit(1);
    }
    if (my_rank == 0){
        // load the problem
        if (myFile){
            std::ifstream file;
            file.open(myFile);
            if (file.is_open()){
                problem->loadProblem(file);
                file.close();
            }
        } else {
            problem->loadProblem(std::cin);
        }
    }
    if (go || my_rank == 0){
        clock_t begin = clock();
        #if defined(_OPENMP)
        double beginR = omp_get_wtime();
        #endif
        problem->solve();

        clock_t end = clock();
        #if defined(_OPENMP)
        double endR = omp_get_wtime();
        #endif
        if (my_rank == 0){
            solution best = problem->getBest();
            best.printSolution();
        }
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "Solution proc time: "<< elapsed_secs << " s." << std::endl;
        #if defined(_OPENMP)
        std::cout << "Solution real time (if parralel): "<< endR - beginR << " s." << std::endl;
        #endif
        delete problem;
    }
    /* shut down MPI */
    MPI_Finalize();
}