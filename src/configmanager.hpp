#pragma once

#include <string>
#include <map>
#include <fstream>

class ConfigManager{
    std::string filename;
    std::map<std::string, std::string> options;
    std::fstream file;
public:
    bool open(const char* filename);
    std::string getvarS(const char* n);
    int getvarI(const char* n);
    float getvarF(const char* n);
    void close();
};