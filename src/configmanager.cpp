#include "configmanager.hpp"
#include "defines.hpp"

#include <iostream>
#include <sstream>

bool ConfigManager::open(const char* filename){
    if(LOC_ROOT==NULL){
        file.open(filename, std::ios::in);
    }
    else{
        file.open(std::string(LOC_ROOT+std::string(filename)).c_str(), std::ios::in);
    }
    if(file.good()){
        good=true;
        while(!file.eof()){
            std::string input;
            std::getline(file, input);
            std::stringstream line(input);
            if(line.str().size()>=1){
                if(line.str().at(0)!='#'){
                    std::string segment1, segment2;
                    std::getline(line, segment1, '=');
                    std::getline(line, segment2, '=');
                    options[segment1] = segment2;
                }
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool ConfigManager::isGood(){
    return good;
}

ConfigManager::~ConfigManager(){
    
}

std::string ConfigManager::getvarS(const char* n){
    return options[n];
}

int ConfigManager::getvarI(const char* n){
    return atoi(options[n].c_str());
}

float ConfigManager::getvarF(const char* n){
    return atof(options[n].c_str());
}