#ifndef UTIL_H
#define UTIL_H
#pragma once
#include <string>
#include <fstream>

class Util {
    public:
        static std::string convertToTime(long int input_seconds);
        static std::string getProgressBar(std::string percent);
        static std::ifstream getStream(std::string path);
};

#endif 
