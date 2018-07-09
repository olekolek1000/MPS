#pragma once

#include <string>
#include "lib/sdl.hpp"
#include "globalevent.hpp"

class sceneEditor;
class Toolbox;
class Drawer;
class colorSelector;
class FrameManager;

class Tool{
private:

public:
    sceneEditor * scene;
    Toolbox * toolbox;
	Drawer * drawer;
	colorSelector * colorselector;
	FrameManager * frameMan;
	
    std::string iconName="undefined";
	std::string iconLocation;
	std::string iconHeader;
    std::string getIconName();

    void initBase(sceneEditor * scene);

    virtual void init(){};
    virtual void eventMouseMove(int x, int y){};
    virtual void eventMouseDown(){};
    virtual void eventMouseUp(){};
    virtual bool pushEvent(SDL_Event * evt){return false;};
    virtual void pushGlobalEvent(GlobalEvent evt){};
    virtual void render(float alpha){};
    virtual void update(){};
    virtual void select(){};
    virtual ~Tool(){};
};

