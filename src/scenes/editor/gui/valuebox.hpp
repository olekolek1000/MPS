#pragma once

#include "lib/sdl.hpp"

class sceneEditor;
class GuiButton;
class GuiText;

typedef unsigned int uint;

class GuiValueBox{
private:
    int posX;
    int posY;
    int width;
    int height;
	
	int minvalue=0;
	int maxvalue=100;
	int value=1;
	
	bool needupdatesize=true;
	bool valuechanged=true;

	GuiButton *but_up;
    GuiButton *but_down;
    GuiText *text_value;
    int holdtime=0;
	sceneEditor * scene;

	void initInterface();
	void updateSize();
	void updateValue();

public:
    GuiValueBox & init(sceneEditor * scene);
    GuiValueBox & init(sceneEditor * scene, int posX, int posY, int width, int height);

    GuiValueBox & setPosition(int posX, int posY);
    GuiValueBox & setSize(int width, int height);

    int getWidth();
    int getHeight();
    int getPosX();
    int getPosY();
	
	GuiValueBox & setMinValue(int n);
	GuiValueBox & setMaxValue(int n);
	int getMinValue();
	int getMaxValue();
	int getValue();
	GuiValueBox & setValue(int n);

	GuiValueBox & update();
    GuiValueBox & render();
    bool pushEvent(SDL_Event * evt);

	GuiValueBox();
    ~GuiValueBox();
};
