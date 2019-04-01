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
    this->write("successfully init the logger. Level: " + std::to_string(this->level));
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
    this->outfile << std::setfill('0') << std::setw(2)<< ms.count() / 1000 % 60 <<"."<< std::setw(3)<< ms.count()%1000 << ";";
}



void LOGGER::write(std::string message){
    this->outfile << this->rank << ";" << this->random << ";" << message << std::endl;
    this->outfile.flush();
}

void LOGGER::info(std::string message){
    if (this->level >= INFO ){
        this->writeTimestamp();
        this->outfile << "INFO;";
        this->write(message);
    }
}

void LOGGER::warn(std::string message){
    if (this->level >= WARN ){
        this->writeTimestamp();
        this->outfile << "WARN;";
        this->write(message);
    }
}

void LOGGER::error(std::string message){
    if (this->level >= ERROR ){
        this->writeTimestamp();
        this->outfile << "ERROR;";
        this->write(message);
    }
}

void LOGGER::debug(std::string message){
    if (this->level >= DEBUG ){
        this->writeTimestamp();
        this->outfile << "DEBUG;";
        this->write(message);
    }
}
