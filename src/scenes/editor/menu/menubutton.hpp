#pragma once

#include <string>
#include "../gui/text.hpp"
#include "lib/glm.hpp"
#include "lib/sdl.hpp"//SDL_Event

class sceneEditor;
class Menu;

class MenuButton{
	glm::mat4 model;
	sceneEditor * scene;
	Menu * menu;
	int posX;
	int posX_prev;
	float posX_smooth;

	int posY;
	int posY_prev;
	float posY_smooth;

	char side;

	float scale;
	float scale_speed;
	float scale_prev;
	float scale_target;

	int width;
	int height;
	GuiText text;
	bool pressed=false;
    bool clicked=false;
	bool hovered=false;
	float objX,objY;
public:
	void init(Menu * menu, int posX, int posY, char side, std::string text);
	void update();
	void render(float alpha);
	void updateObjPos();
	void setPosition(int posX, int posY);
	void setXPosition(int posX);
	void setYPosition(int posY);
	void setSide(char side);//A-I
	/*
	A===B===C
	|.......|
	H...I...D
	|.......|
	G===F===E
	*/
	bool pushEvent(SDL_Event * evt);
	bool isClicked();
	bool isPressed();
};