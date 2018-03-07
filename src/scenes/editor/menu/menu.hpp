#pragma once

#include "lib/glm.hpp"
#include "texturemanager.hpp"
#include "timestep.hpp"

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
	
	bool wasFullscreen;
	void dialogPrepare();
	void dialogEnd();
	
	int fileVersion = 1;
	const char * filePrefix = "Moving Picture Studio Project";
	
	void actionBack();
	void actionExport();
	void actionLoadProject();
	void actionSaveProject();
	void actionQuit();
	
	Timestep step;
public:
	App * a;
	sceneEditor * scene;
	void loop();
	Menu(sceneEditor * scene);
	~Menu();
};