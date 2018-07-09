#pragma once

#include "tool.hpp"
#include "scenes/editor/gui/valuebox.hpp"
#include "scenes/editor/gui/button.hpp"

class Resize : public Tool{
    bool down=false;
    int x,y,prevX,prevY;
    void draw();

	GuiValueBox box_width;
    GuiValueBox box_height;
    GuiButton but_apply;
public:
    void init();
    bool pushEvent(SDL_Event * evt);
    void update();
    void render(float alpha);
    void select();
};
