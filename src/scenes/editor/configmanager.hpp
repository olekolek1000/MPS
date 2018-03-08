#pragma once
#include <fstream>
#include <string.h>
#include <vector>

class configManager {
public:
    ~configManager();
    std::string getVariable(std::string variableName);
    bool loadFile(std::string filename);
private:
    std::fstream file;
    std::vector<std::string> fileLines;
};