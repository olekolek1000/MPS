#pragma once

#include "tool.h"
#include "scenes/editor/gui/valuebox.h"

class Spray : public Tool{
    bool down=false;
    int x,y;
    void draw();
	
	GuiValueBox box_size;
	GuiValueBox box_intensity;
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
