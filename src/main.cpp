//
// Created by martilad on 3/14/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include "../headers/solution.h"
#include "../headers/solvers/solver.h"
#include "../headers/solvers/recursion.h"
#include "../headers/solvers/dataParallel.h"
#include "../headers/solvers/taskParallel.h"

int main(int argc, char* argv[]) {
    char *myFile = nullptr;
    bool stdIn = false;
    int run = 0;
    int nThreads = -1;
    int deep = -1;
    int nInst = -1;
    // load the arguments
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && (i + 1 <= argc))  {
            if (i == argc-1){
                std::cout << "Specific the file!!!";
                exit(1);
            }
            myFile = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            std::cout << "Use -f <file> for the load from file or -i switch if you want to standard input." << std::endl;
            exit(0);
        } else if (strcmp(argv[i], "-deep") == 0) {
            if (i == argc-1){
                std::cout << "Insert depth for add recursion as task for parallel!!!";
                exit(1);
            }
            deep = std::stoi(argv[i + 1]);
            i++;
        }else if (strcmp(argv[i], "-nInst") == 0) {
            if (i == argc-1){
                std::cout << "Insert number of task generated for data parallel algorithm!!!";
                exit(1);
            }
            nInst = std::stoi(argv[i + 1]);
            i++;
        }else if (strcmp(argv[i], "-nThreads") == 0) {
            if (i == argc-1){
                std::cout << "Insert number of threads!!!";
                exit(1);
            }
            nThreads = std::stoi(argv[i + 1]);
            i++;
        }else if (strcmp(argv[i], "-i") == 0) {
            stdIn = true;
        }else if (strcmp(argv[i], "-dp") == 0){
            run = 1;
        }else if (strcmp(argv[i], "-r") == 0){
            run = 2;
        }else if (strcmp(argv[i], "-tp") == 0){
            run = 3;
        } else {
            std::cout << "Not enough or invalid arguments, please try again." << std::endl;
            exit(1);
        }
    }
    Solver * problem;
    switch (run){
        case 1:
            if (nThreads < 1 || nInst < 0){
                std::cout << "Specific real number of thread (-nThreads) or positive number of generated tasks (-nInst)." << std::endl;
                exit(1);
            }
            problem = new DataParallel(nThreads, deep);
            break;
        case 2:
            problem = new Recursion();
            break;
        case 3:
            if (nThreads < 1 || deep < 0){
                std::cout << "Specific real number of thread (-nThreads) or positive number of generated tasks (-deep)." << std::endl;
                exit(1);
            }
            problem = new TaskParallel(nThreads, deep);
            break;
        default:
            std::cout << "Please specific the algorithm: \"-dp\" data parallel solution;";
            std::cout << ", \"-r\" recursion with share memory (the fastest single thread implementation), ";
            std::cout << "\"-tp\" task parallelism recursion with copying." << std::endl;
            exit(1);
    }

    // load the problem
    if (myFile){
        std::ifstream file;
        file.open(myFile);
        if (file.is_open()){
            problem->loadProblem(file);
            file.close();
            stdIn = false;
        }
    }
    if (stdIn) problem->loadProblem(std::cin);

    if (problem->isLoad()){
        clock_t begin = clock();
        #if defined(_OPENMP)
        double beginR = omp_get_wtime();
        #endif

        problem->solve();
        clock_t end = clock();
        #if defined(_OPENMP)
        double endR = omp_get_wtime();
        #endif

        solution best = problem->getBest();
        best.printSolution();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "Solution proc time: "<< elapsed_secs << " s." << std::endl;
        #if defined(_OPENMP)
        std::cout << "Solution real time (if parralel): "<< endR - beginR << " s." << std::endl;
        #endif
        delete problem;
    }else{
        std::cout << "Problem not load. Need use -i for load problem from standard input or -f and specific file." << std::endl;
    }
}