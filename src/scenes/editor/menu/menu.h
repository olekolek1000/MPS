#pragma once

#include "lib/glm.h"

class App;
class sceneEditor;

class Menu{
private:
	App * a;
	sceneEditor * scene;
	glm::mat4 projection, model;
	void setProjection();
	void exitMenu();
	float x_target = -1.3;
	float y_target = 0.0;
	float z_target = 0.5;
	float animspeed = 0.08;
	int exit_delay=0;
public:
	void loop();
	Menu(sceneEditor * scene);
	~Menu();
};