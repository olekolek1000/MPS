#pragma once

#include "tool.h"

class Pipette : public Tool{
    int x, y;
    int mouseX,mouseY;
    bool isMoved;
    float opacity;
    int pixel,pixel_prev;
    float scaleMul;
public:
    void init();
    void eventMouseMove(int x, int y);
    void eventMouseDown();
    void eventMouseUp();
    bool pushEvent(SDL_Event * evt);
    void update();
    void render();
    void select();
};
