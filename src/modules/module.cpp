#include "module.hpp"
#include <iostream>

Module::~Module() {
    unload();
}

bool Module::unload() {
    if(loaded) {
        free_library(libhandle);
        libhandle = 0;
        loaded = false;
        return true;
    }
    return false;
}

Module::Module(const char* path) {
    loaded = false;
    load(path);
}

Module& Module::operator= (Module& __arg__) {
     unload();
     loaded = __arg__.loaded;
     libhandle = __arg__.libhandle;

     __arg__.libhandle = 0;
     __arg__.loaded = false;
    
     return *this;
} 

bool Module::load(const char* path) {
    libhandle_t ptr;
    if((ptr = load_library(path)) != NULL) {
        if(loaded) {
            free_library(libhandle);
        }
        libhandle = ptr;
        loaded = true;

        return true;
    }
    return false;
}

void* Module::get_function(const char* name) {
    return load_function(libhandle,name);
}

bool Module::isLoaded() {
    return loaded;
}
