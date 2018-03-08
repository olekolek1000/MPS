#pragma once

#ifdef _WIN32
    #include <windows.h>
    typedef HINSTANCE libhandle_t
#else
    #include <dlfcn.h>
    typedef void* libhandle_t;
#endif

static inline libhandle_t load_library(const char* path) {
    libhandle_t libhandle_temp;
    #ifdef _WIN32
        libhandle_temp = LoadLibrary(path);
    #else
        libhandle_temp = dlopen(path,RTLD_LAZY);
    #endif
    return libhandle_temp;
}

static inline void* load_function(libhandle_t hand, const char* name) {
    #ifdef _WIN32
        return GetProcAddress(hand,name);
    #else
        return dlsym(hand,name);
    #endif
    return f;
}

static inline int free_library(libhandle_t hand) {
    #ifdef _WIN32
        return FreeLibrary(hand);
    #else
        return dlclose(hand);
    #endif
}

