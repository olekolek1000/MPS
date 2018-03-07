#pragma once

#include "lib/glm.hpp"
#include "texturemanager.hpp"
#include "tool/tool.hpp"
#include "scenes/editor/gui/text.hpp"
#include "scenes/editor/gui/button.hpp"
#include <vector>

typedef unsigned int uint;

class sceneEditor;

class Toolbox{
private:
    int toolboxWidth=30*4,toolboxHeight=100*4;

    sceneEditor * scene;
    glm::mat4 model;

    GuiText textCurrentTool;
    GuiText textClock;
    int clockLast=0;

    Tool * currentTool;
    Tool * previousTool;

    int settingsX = 0;
    int settingsY = 0;
    int settingsH = 0;


    GuiButton butUndo;
    GuiButton butRedo;
    GuiButton butMenu;

	std::vector<GuiButton*>buttons;
    std::vector<Tool*>tools;
    void createTool(Tool * tool);
public:
    void init(sceneEditor * scene);
    void update();
    void render();
    Tool * getCurrentTool();
    Tool * getPreviousTool();
    void setCurrentTool(Tool * tool);
    void setCurrentTool(uint index);
    bool pushEvent(SDL_Event * evt);
	void reloadTextures();

	int getWidth();
    int getSettingsX();
    int getSettingsY();
    void setSettingsHeight(int n);
    ~Toolbox();
};
