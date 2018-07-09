#pragma once

#include "tool.hpp"

class FloodFill : public Tool{
    int x,y;
    int startX, startY;
    int oldcolor;
public:
    void init();
    void floodfill(int posX, int posY);
    void eventMouseMove(int x, int y);
    void eventMouseDown();
    void eventMouseUp();
    bool pushEvent(SDL_Event * evt);
    void update();
    void render(float alpha);
    void select();
};
