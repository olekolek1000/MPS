#pragma once

#include "tool.hpp"
#include "scenes/editor/gui/valuebox.hpp"

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
    void render(float alpha);
    void select();
};
