#pragma once

#include "lib/sdl.hpp"
#include "lib/opengl.hpp"

typedef unsigned int uint;

class Frame{
private:
    friend class Drawer;
    friend class FrameManager;

    SDL_Surface * overlay;
    uint overlayTex;

    int width;
    int height;

    int index;

    bool loaded;
    bool needUpdate;
    //int updateTop,updateBottom,updateLeft,updateRight;

    void initvars();
public:
    Frame();
    void free();
    void load(int w, int h);
    bool isLoaded();
    void bindTexture();
    int getWidth();
    int getHeight();
    SDL_Surface * getOverlay();
    float getProportions();
    int getIndex();
	void forceUpdate();
    ~Frame();
};
