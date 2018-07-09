#pragma once

#include "lib/sdl.hpp"

class sceneEditor;

class GuiCheckbox{
private:
    struct Pimpl;
    Pimpl * p;
public:
    GuiCheckbox();
    GuiCheckbox(sceneEditor * s, int posX=0, int posY=0, int size=16);
    GuiCheckbox& init(sceneEditor * s, int posX=0, int posY=0, int size=16);
    ~GuiCheckbox();
    
    bool pushEvent(SDL_Event * evt);
    GuiCheckbox& update();
    GuiCheckbox& render(float alpha);
};