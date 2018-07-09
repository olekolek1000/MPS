#pragma once

#include "lib/glm.hpp"
#include "texturemanager.hpp"
#include "timestep.hpp"

#include "menubutton.hpp"
#include <map>
class App;
class sceneEditor;


class Menu{
private:
	friend class ExporterGIF;

	glm::mat4 projection, model;
	void setProjection();
	void exitMenu();
	float x_target = -1.2;
	float y_target = 0.0;
	float z_target = 0.0;
	float animspeed = 0.1;
	int exit_delay_start=12;
	int exit_delay=0;
	bool projectionChanged=false;
	
	bool wasFullscreen;
	void dialogPrepare();
	void dialogEnd();
	
	int fileVersion = 2;
	const char * filePrefix = "Moving Picture Studio Project";
	
	void actionBack();
	void actionImport();
	void actionExport();
	void actionLoadProject();
	void actionSaveProject();
	void actionExit();
	void createLoading();
	void updateLoadingStatus(int frame);
	void freeLoading();
	SDL_Window * loadingWindow;
	
	std::map<std::string, MenuButton> buttons;

	Timestep step;
public:
	App * a;
	sceneEditor * scene;
	void loop();
	Menu(sceneEditor * scene);
	~Menu();
};