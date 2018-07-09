#pragma once

#include "tool.hpp"

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
    void render(float alpha);
    void select();
};
