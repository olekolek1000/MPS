#pragma once

#include "lib/glm.h"
#include "texturemanager.h"

class App;
class sceneEditor;

class Menu{
private:
	glm::mat4 projection, model;
	void setProjection();
	void exitMenu();
	float x_target = -1.3;
	float y_target = 0.0;
	float z_target = 0.5;
	float animspeed = 0.08;
	int exit_delay=0;
public:
	App * a;
	sceneEditor * scene;
	void loop();
	Menu(sceneEditor * scene);
	~Menu();
};