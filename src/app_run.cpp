#include "app.hpp"
#include "error.hpp"

#include "scenes/editor/editor.hpp"
#include "defines.hpp"

#include <sstream>

#ifdef __linux__ //Only for Linux™

#include <signal.h>
#include <execinfo.h>
#include <unistd.h>

void signalHandler(int sig){
    stringstream ss;
    ss<<"PROGRAM CRASH, signal "<<sig<<"\n See stderr (program output) for more info.";

    void *array[15];
    size_t size;
    size = backtrace(array, 15);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    
    error(ss.str());
    exit(-1);
}

#endif


void App::runApplication() {
    #ifdef __linux__
    signal(SIGSEGV, signalHandler);
    #endif
    while(true) {
        if(true){
            sceneEditor editor(*this);
            SDL_ShowWindow(window);
            #ifndef FASTSTART
            if(loadingwindow!=NULL){
                SDL_DestroyWindow(loadingwindow);
                loadingwindow=NULL;
            }
            #endif
            SDL_GL_MakeCurrent(window, context);
            editor.loop();
        }
    }
}
