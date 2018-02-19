#include "app.h"

#include "scenes/editor/editor.h"
#include "defines.h"

void App::runApplication() {
    SDL_SetWindowMinimumSize(window, 1280, 720);

    while(true) {
        if(true){
            sceneEditor editor(*this);
            SDL_ShowWindow(window);
            #ifndef FASTSTART
            SDL_DestroyWindow(loadingwindow);
            #endif
            SDL_GL_MakeCurrent(window, context);
            editor.loop();
        }
    }
}
