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
#include "../headers/POL.h"
#include "../headers/solvers/solver.h"
#include "../headers/solvers/recursion.h"

int main(int argc, char* argv[]) {
    char *myFile = nullptr;
    bool stdIn = false;
    int run = 0;
    int nThreads = 1;
    int deep = 1;
    // load the arguments
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && (i + 1 <= argc))  {
            myFile = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            std::cout << "Use -f <file> for the load from file or -i switch if you want to standard input." << std::endl;
            exit(0);
        } else if (strcmp(argv[i], "-deep") == 0) {
            deep = std::stoi(argv[i + 1]);
            i++;
        }else if (strcmp(argv[i], "-nThreads") == 0) {
            nThreads = std::stoi(argv[i + 1]);
            i++;
        }else if (strcmp(argv[i], "-i") == 0) {
            stdIn = true;
        }else if (strcmp(argv[i], "-ls") == 0){
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
            std::cout << "Not implemet" << std::endl;
            break;
        case 2:
            problem = new Recursion();
            break;
        case 3:
            std::cout << "Not implemet" << std::endl;
            break;
        default:
            std::cout << "Please specific the algorithm: \"-ls\" solution wth local stack;";
            std::cout << ", \"-r\" recursion with share memory, ";
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
        std::cout << "Problem not load." << std::endl;
    }
}