//
// Created by martilad on 3/24/19.
//

#ifndef MI_PDP2019_LOGGER_H
#define MI_PDP2019_LOGGER_H
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>


enum LEVEL {
    INFO = 2, DEBUG = 3, ERROR = 0, WARN = 1
};


class LOGGER{

    std::string file_name;
    int rank;
    int level;
    int random;
    std::ofstream outfile;
    void writeTimestamp();
    void writeLevel();
    void write(std::string message);

public:
    LOGGER(int rank, std::string baseFileName, std::string path = "./", int LEVEL = INFO);
    ~LOGGER();

    void info(std::string message);
    void warn(std::string message);
    void error(std::string message);
    void debug(std::string message);

};

#endif //MI_PDP2019_LOGGER_H
