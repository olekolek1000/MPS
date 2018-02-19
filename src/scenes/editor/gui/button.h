#pragma once

#include "lib/sdl.h"

class sceneEditor;
class Texture;

typedef unsigned int uint;

class GuiButton{
private:
    int posX;
    int posY;
    int width;
    int height;

	bool pressed=false;
    bool clicked=false;
    bool background=true;

    void * model = NULL;
    sceneEditor * scene = NULL;
    Texture * texture = NULL;
public:
	GuiButton();
    ~GuiButton();

    GuiButton & init(sceneEditor * scene);
    GuiButton & init(sceneEditor * scene, int posX, int posY, int width, int height);

    GuiButton & setTexture(Texture * texture);

    GuiButton & setPosition(int posX, int posY);
    GuiButton & setSize(int width, int height);
    GuiButton & setBackgroundVisible(bool n);

    int getWidth();
    int getHeight();
    int getPosX();
    int getPosY();

    GuiButton & render();
    bool pushEvent(SDL_Event * evt);
    bool isClicked();
    bool isPressed();
};
