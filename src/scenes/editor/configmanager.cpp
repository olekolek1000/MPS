#include "configmanager.hpp"
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include "../../error.hpp"

bool configManager::loadFile(std::string filename) {
    fileLines.clear();
    file.open(filename.c_str(), std::ios::in);

    if(!file.good()) {
        error("Can't open "+filename);
        return false;
    } else {
        std::string confLine;
        while (getline (file,confLine) ) {
            fileLines.push_back(confLine);
        }
        return true;
    }
}
configManager::~configManager() {
    file.close();
}
std::string configManager::getVariable(std::string variableName) {
    std::string fromVar;
    for(int i = 0; i < fileLines.size(); i++) {
        std::string cmplted;
        for(int j = 0; j < fileLines[i].length(); j++) {
            cmplted += fileLines[i].at(j);
            if(cmplted==variableName+"=") {
                for(int h = j + 1; h < fileLines[i].length(); h++) {
                    if(fileLines[i].at(h) == ';') {
                        break;
                    }
                    fromVar += fileLines[i].at(h);
                }
                break;
            }
        }
        if(cmplted==variableName+"=") {
            break;
        }
    }
    return fromVar;
}
