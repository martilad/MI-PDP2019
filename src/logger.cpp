//
// Created by martilad on 3/24/19.
//

#include "../headers/logger.h"

LOGGER::LOGGER(int rank, std::string baseFileName, std::string path, int LEVEL){
    this->rank = rank;
    this->level = LEVEL;
    this->random = rand();
    this->file_name = path + std::to_string(this->rank) + baseFileName + ".log";
    this->outfile.open(this->file_name);
    this->write("successfully init the logger");
}

LOGGER::~LOGGER(){
    // close the opened file.
    outfile.close();
}

void LOGGER::writeTimestamp(){
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    this->outfile << std::setfill('0') << std::setw(2)<< ms.count() / 3600000 % 24 << ":";
    this->outfile << std::setfill('0') << std::setw(2)<< ms.count() / 60000 % 60  <<":";
    this->outfile << std::setfill('0') << std::setw(2)<< ms.count() / 1000 % 60 <<"."<< ms.count()%1000 << ";";
}

void LOGGER::writeLevel(){
    switch (this->level){
        case INFO:
            this->outfile << "INFO;";
            break;
        case ERROR:
            this->outfile << "ERROR;";
            break;
        case WARN:
            this->outfile << "WARN;";
            break;
        case DEBUG:
            this->outfile << "DEBUG;";
            break;
        default:
            this->outfile << "LOG ERROR -> FAIL LEVEL;";
            break;
    }

}

void LOGGER::write(std::string message){
    this->writeTimestamp();
    this->writeLevel();
    this->outfile << this->rank << ";" << this->random << ";" << message << std::endl;
    this->outfile.flush();
}

void LOGGER::info(std::string message){
    if (this->level >= INFO )this->write(message);
}

void LOGGER::warn(std::string message){
    if (this->level >= WARN )this->write(message);
}

void LOGGER::error(std::string message){
    if (this->level >= ERROR )this->write(message);
}

void LOGGER::debug(std::string message){
    if (this->level >= DEBUG )this->write(message);
}
