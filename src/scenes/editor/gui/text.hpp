#pragma once

#include <string>
#include "lib/sdl_ttf.hpp"

class sceneEditor;

typedef unsigned int uint;

class GuiText{
private:
    int width;
    int height;
    bool loaded=false;
    std::string text;
    uint texture;
    void * model;
    sceneEditor * scene;
    int posX;
    int posY;
    char alignX=false;
    char alignY=false;
    float angle=0.0;
	void construct();
public:
    GuiText();
    GuiText& init(sceneEditor * scene);
    GuiText(sceneEditor * scene);
    GuiText(sceneEditor * scene, std::string text, TTF_Font * font, Uint8 r, Uint8 g, Uint8 b);
    GuiText& changeText(std::string text, TTF_Font * font, Uint8 r, Uint8 g, Uint8 b);
    int getWidth();
    int getHeight();
    float getProportions();

    GuiText& render();
    GuiText& setRotation(float angle);
    GuiText& setPosition(int x, int y);
	
	GuiText& setAlign(char x, char y);
	GuiText& setXAlign(char n);
	GuiText& setYAlign(char n);

    void free();
    ~GuiText();
};
