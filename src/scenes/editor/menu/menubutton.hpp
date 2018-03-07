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
	float scale;
	float scale_prev;
	float scale_target;
	int posY;
	int width;
	int height;
	GuiText text;
	bool pressed=false;
    bool clicked=false;
	bool hovered=false;
public:
	void init(Menu * menu, int posY, std::string text);
	void update();
	void render(float alpha);
	void setXPosition(int posX);
	bool pushEvent(SDL_Event * evt);
	bool isClicked();
	bool isPressed();
};