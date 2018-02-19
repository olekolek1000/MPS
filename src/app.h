#pragma once

#include "lib/sdl_ttf.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "framebuffermanager.h"

#include <vector>

#include "render/buffer.h"

typedef unsigned int uint;

enum class GlobalEvent {
    Nothing,
    WindowSizeChange,
    Exit
};

class App {
private:
	bool hidden=false;
    int window_w_prev=-1,window_h_prev=-1;
    glm::mat4 model, projection;
    void resizeFrameBuffers();
    int window_w,window_h;
    int fb_w, fb_h; 
    std::vector<GlobalEvent>events;
    void pushEvent(GlobalEvent event);
    float proportions;

    bool fullscreen=true;
    bool vsync=true;
    bool autoproportions=true;

    int areawidth=1, areaheight=1;
    float areamultipler=1.0;

    uint fps_last=0;
    uint fps_frames=0;
    uint fps=0;
    bool debugger=false;
    Texture fps_text;

    const Uint8 * keystate;

    void updateProportions();
    void updateDebugger();
    void runApplication();//app_run.cpp
public:
    TTF_Font * font10;
    TTF_Font * font16;
    TTF_Font * font24;
    TTF_Font * font32;
    TTF_Font * font64;
    TTF_Font * font128;
    RBuffer *square_vert, *square_uv;
    SDL_Window * window;
    ShaderManager shMan;

    int getWindowWidth();
    int getWindowHeight();
    float getProportions();

    void setAutoProportions(bool n);
    bool getAutoProportions();

    int getAreaWidth();
    int getAreaHeight();
    void setAreaMultipler(float n);
    void setProportions(float prop);
    bool onEvent();
    GlobalEvent getEvent();
    bool isFullscreen();
    void setFullscreen(bool state);
    bool isPressed(int scancode);
	bool isHidden();

    void init();
    void updateAll();
    void updateWindow();
    void blitFramebuffer(Framebuffer * buffer);
    bool eventHandle(SDL_Event * evt);

    //Moving Picture Studio VVV
    SDL_Window * loadingwindow;
    SDL_GLContext context;
};
