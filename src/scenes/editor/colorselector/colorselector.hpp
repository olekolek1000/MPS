#pragma once

#include "texturemanager.hpp"
#include "lib/glm.hpp"
#include "../gui/text.hpp"

typedef unsigned int uint;

class sceneEditor;

class colorSelector{
    sceneEditor * scene;
    glm::mat4 model;

    Texture verticalbartexture;
    Texture colorstexture;

    GuiText text_info;

    int hue=0;
    int saturation=0;
    int value=0;

    int red=0,green=0,blue=0;

    int colorsX;

    bool updatecolors=false;
    bool ischanginghue=false;
    bool ischangingcolor=false;

    int selectorheight = 180;

    bool down=false;

    void updateColorsTexture();

    void changeHueToCursorPosition(int y);
    void changeColorToCursorPosition(int x, int y);
    void updateRGB();
    void updateText();


    bool used=false;//events
public:
    void init(sceneEditor * scene);

    void setColor(Uint8 r, Uint8 g, Uint8 b);

    void render();
    bool pushEvent(SDL_Event * evt);
    ~colorSelector();
};
