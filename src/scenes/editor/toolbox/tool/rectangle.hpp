#pragma once

#include "tool.hpp"
#include "scenes/editor/gui/valuebox.hpp"

class Rectangle : public Tool{
    bool down=false;
    int startX, startY, currentX, currentY;
    void draw();

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
