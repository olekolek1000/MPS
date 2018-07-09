#pragma once

#include "lib/sdl.hpp"
#include "lib/sdl_ttf.hpp"

#include <string>

class sceneEditor;
class Texture;
class GuiText;

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

    GuiText * text = NULL;

    float scale = 1.0f;
    float scale_smooth = 1.0f;
    float scale_smooth_prev = 1.0f;
    bool hovered=false;

    void * model = NULL;
    sceneEditor * scene = NULL;
    Texture * texture = NULL;
public:
	GuiButton();
    ~GuiButton();

    GuiButton & init(sceneEditor * scene);
    GuiButton & init(sceneEditor * scene, int posX, int posY, int width, int height);

    GuiButton & setTexture(Texture * texture);
    GuiButton & setText(std::string text, TTF_Font * font, char r, char g, char b);

    GuiButton & setPosition(int posX, int posY);
    GuiButton & setSize(int width, int height);
    GuiButton & setBackgroundVisible(bool n);

    int getWidth();
    int getHeight();
    int getPosX();
    int getPosY();

    GuiButton & update();
    GuiButton & render(float alpha);
    bool pushEvent(SDL_Event * evt);
    bool isClicked();
    bool isPressed();
};
