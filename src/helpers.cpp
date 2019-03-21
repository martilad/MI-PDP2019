//
// Created by martilad on 3/14/19.
//
#include "../headers/helpers.h"

namespace helpers{
    int eval_pol(int number) {
        if (number == 1) return -6;
        if (number == 2) return -12;
        if (number == 3) return -18;
        if (number == 6) return -3;
        if (number == 7) return -9;
        if (number == 11) return 0;
        int mod = number % 5;
        if (mod == 0) return (number / 5) * 3;
        else if (mod == 1) return ((number - 16) / 5) * 3 + 4 * 2;
        else if (mod == 2) return ((number - 12) / 5) * 3 + 3 * 2;
        else if (mod == 3) return ((number - 8) / 5) * 3 + 2 * 2;
        else return ((number - 4) / 5) * 3 + 2;
    }


    void load_arguments(char **file, int *run, int *nT, int *nN, int *nNP, int argc, char* argv[]){
        // load the arguments
        for (int i = 1; i < argc; i++) {
            if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && (i + 1 <= argc))  {
                if (i == argc-1){
                    std::cout << "Specific the file!!!";
                    exit(1);
                }
                (*file) = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                std::cout << "Use -f <file> for the load from file not if you want to standard input.";
                std::cout << "There are many others switch, need write this TODO:" << std::endl;
                exit(0);
            } else if (strcmp(argv[i], "-nN") == 0) {
                if (i == argc-1){
                    std::cout << "Insert depth or min number of inst for split problem in one process!!!";
                    exit(1);
                }
                (*nN) = std::stoi(argv[i + 1]);
                i++;
            }else if (strcmp(argv[i], "-nNP") == 0) {
                if (i == argc-1){
                    std::cout << "Insert min number of generated instances with master process in MPI.!!!";
                    exit(1);
                }
                (*nNP) = std::stoi(argv[i + 1]);
                i++;
            }else if (strcmp(argv[i], "-nT") == 0) {
                if (i == argc-1){
                    std::cout << "Insert number of threads!!!";
                    exit(1);
                }
                (*nT) = std::stoi(argv[i + 1]);
                i++;
            }else if (strcmp(argv[i], "-dp") == 0){
                (*run) = 1;
            }else if (strcmp(argv[i], "-r") == 0){
                (*run) = 2;
            }else if (strcmp(argv[i], "-tp") == 0){
                (*run) = 3;
            }else if (strcmp(argv[i], "-mpi") == 0){
                (*run) = 4;
            } else {
                std::cout << "Not enough or invalid arguments, please try again." << std::endl;
                exit(1);
            }
        }
    }

}


