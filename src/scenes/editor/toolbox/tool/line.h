#pragma once

#include "tool.h"
#include "scenes/editor/gui/valuebox.h"

class Line : public Tool{
    int x,y;
    int startX, startY;
    bool down=false;
    bool moved=false;

    GuiValueBox box;
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
