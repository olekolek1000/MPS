#pragma once

#include "lib/glm.hpp"
#include "texturemanager.hpp"
#include "shadermanager.hpp"
#include "framebuffermanager.hpp"
#include "app.hpp"
#include "timestep.hpp"
#include "framemanager.hpp"
#include "thememanager.hpp"
#include "toolbox/toolbox.hpp"
#include "drawer.hpp"
#include "colorselector/colorselector.hpp"
#include "frameselector/frameselector.hpp"
#include "layermanager.hpp"
#include "actionlog.hpp"
#include <map>

class sceneEditor{
    void load();
	bool menuopenrequest=false;
public:
    App &a; 
	sceneEditor(App & app) : a(app){load();};

    glm::mat4 guiProjection, projection, model;

    Shader shGui;
	Shader shGuiAlpha;
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
	LayerManager layerMan;
	ActionLog actionlog;
	std::map<std::string, SDL_Cursor*>cursors;
	void setCursor(std::string name);
	void openMenu();

	void setProjection();
	void loop();

	void changeFrame(int n);

	~sceneEditor();
};
