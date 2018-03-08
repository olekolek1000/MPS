#pragma once

#include "lib/glm.hpp"
#include "texturemanager.hpp"
#include "timestep.hpp"
#include "../../../modules/module.hpp"

class App;
class sceneEditor;

class pix_map {
public:
	int** pixmaps;
	int* heights;
	int* weights;

	void Free();
	void Malloc(size_t size);
};

class Menu{
private:
	pix_map current_export_data;

	Module::Manager_t io_library_manager;
	float current_export_thread_progress;

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
	void actionImport();
	void actionExport();
	void actionLoadProject();
	void actionSaveProject();
	void actionGoSettings();
	void actionQuit();

	bool LoadModule();
	
	Timestep step;
public:
	App * a;
	sceneEditor * scene;
	void loop();
	Menu(sceneEditor * scene);
	~Menu();
};