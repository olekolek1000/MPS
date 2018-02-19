#pragma once

#include "lib/glm.h"
#include "texturemanager.h"
#include "shadermanager.h"
#include "framebuffermanager.h"
#include "app.h"
#include "timestep.h"
#include "framemanager.h"
#include "thememanager.h"
#include "toolbox/toolbox.h"
#include "drawer.h"
#include "colorselector/colorselector.h"
#include "frameselector/frameselector.h"
#include <map>

class sceneEditor{
    void load();
	bool menuopenrequest=false;
public:
    App &a; 
	sceneEditor(App & app) : a(app){load();};

    glm::mat4 guiProjection, projection, model;

    Shader shGui;
	Shader shGuiColor;

	ThemeManager thMan;
	ShaderManager shMan;
	FramebufferManager fbMan;
	Timestep step;
	
	Texture tex_render;

	FrameManager frameMan;
	Drawer drawer;
    Toolbox toolbox;
	colorSelector colorselector;
	frameSelector frameselector;

	std::map<std::string, SDL_Cursor*>cursors;
	void setCursor(std::string name);
	void openMenu();

	void setProjection();
	virtual void loop();

	void changeFrame(int n);

	~sceneEditor();
};
