#pragma once

#include "texturemanager.h"
#include "../gui/text.h"
#include "lib/glm.h"
#include "../gui/button.h"

typedef unsigned int uint;

class sceneEditor;

class frameSelector{
    sceneEditor * scene;
    glm::mat4 model;
    uint linebuffer;
    GuiText text_currentframe;
    GuiButton but_newframe;
    GuiButton but_removeframe;
    GuiButton but_newframetype;
    bool newFrameType=0;//0 = blank, 1 = duplicate

    int areawidth=0,areaheight=0;

    int prev_index=-1;
    float index_difference=0;
    float index_difference_prev=0;

    int topWidth=256;
    int topHeight=48;

    int height = 100;
    bool down=false;
    bool used=false;//events

    void updateCurrentFrameText();
public:
    void init(sceneEditor * scene);

    void update();
    void render(float alpha);
    bool pushEvent(SDL_Event * evt);
    int getHeight();

    ~frameSelector();
};
