#pragma once

#include "lib/sdl_ttf.hpp"
#include "shadermanager.hpp"
#include "texturemanager.hpp"
#include "framebuffermanager.hpp"

#include <vector>

#include "render/buffer.hpp"
#include "globalevent.hpp"
#include "fps.hpp"
#include "configmanager.hpp"

typedef unsigned int uint;

class App {
private:
	bool hidden=false;
    int window_w_prev=-1,window_h_prev=-1;
    glm::mat4 model, projection;
    void resizeFrameBuffers();
    int window_w,window_h;
    int fb_w, fb_h; 
    std::vector<GlobalEvent>events;
    float proportions;
    Fps fpstimer;

    bool fullscreen;
    bool vsync;
    bool autoproportions=true;

    int areawidth=1, areaheight=1;
    float areamultipler=1.0;

    uint fps_last=0;
    uint fps_frames=0;
    uint fps_limit_hz=0;
    bool fps_limit;
    uint fps=0;
    uint64_t fps_frametime = 0;
    uint64_t fps_frametime_last = 0;

    bool debugger = false;
    Texture fps_text;
 
    const Uint8 * keystate;

    void updateProportions();
    void updateDebugger();
    void runApplication();//app_run.cpp
public:
    TTF_Font * font10 = NULL;
    TTF_Font * font16 = NULL;
    TTF_Font * font24 = NULL;
    TTF_Font * font32 = NULL;
    TTF_Font * font64 = NULL;
    TTF_Font * font128 = NULL;
    void reloadFonts();
    RBuffer *square_vert, *square_uv;
    SDL_Window * window;
    ShaderManager shMan;
    ConfigManager config;

    int getWindowWidth();
    int getWindowHeight();
    float getProportions();

    void setAutoProportions(bool n);
    bool getAutoProportions();

    int getAreaWidth();
    int getAreaHeight();
    void setAreaMultipler(float n);
    float getAreaMultipler();
    void setProportions(float prop);
    bool onEvent();
    GlobalEvent getEvent();
    bool isFullscreen();
    void setFullscreen(bool state);
    bool isDebuggerOpened();
    bool isVSyncActive();
    bool isFPSLimitActive();
    uint getFPSLimit();
    bool isPressed(int scancode);
	bool isHidden();
    void pushEvent(GlobalEvent event);
    void setFpsLimit(uint n);
    void makeGLCurrent();

    void init();
    void updateAll();
    void updateWindow();
    void blitFramebuffer(Framebuffer * buffer);
    bool eventHandle(SDL_Event * evt);

    //Moving Picture Studio VVV
    SDL_Window * loadingwindow;
    SDL_GLContext context;
};
