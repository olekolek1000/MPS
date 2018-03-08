#pragma once

#include <map>
#include "load_library.hpp"

class Module {
private:
    libhandle_t libhandle;
    bool loaded;

public:

    bool unload();
    bool load(const char* path);
    void* get_function(const char* name);
    bool isLoaded();


    Module() : loaded(false) {};
    Module(const char* path);
    ~Module();
    
    Module& operator =(Module& __arg__);

    typedef bool(*export_ft)(const int**, const int*, const int*, int, float*); //<- the last argument points to progress float value
    typedef const char*(*info_ft)(void); //<- Returns supported format
    typedef std::map<std::string,Module> Manager_t; //<-Map with Modules
};
