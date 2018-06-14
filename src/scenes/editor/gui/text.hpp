#pragma once

#include <string>
#include "lib/sdl_ttf.hpp"
#include "lib/floats.hpp"

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
    float alpha=1.0;
    float angle=0.0;
    Float4 backgroundColor = {0.0f,0.0f,0.0f,0.0f};
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
    std::string getText();

    GuiText& render();
    GuiText& setRotation(float angle);
    GuiText& setPosition(int x, int y);
    GuiText& setAlpha(float n);
    GuiText& setBackgroundColor(float r, float g, float b, float a);
    GuiText& setBackgroundColor(Float4 n);
	
	GuiText& setAlign(char x, char y);
	GuiText& setXAlign(char n);
	GuiText& setYAlign(char n);

    void free();
    ~GuiText();
};
