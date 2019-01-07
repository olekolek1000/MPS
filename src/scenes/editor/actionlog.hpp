#pragma once

#include <string>
#include <vector>

class sceneEditor;
class GuiText;

#include "globalevent.hpp"

struct ActionCell{
    GuiText * text;
    int starttime;
    float timemultiplier;
};

class ActionLog{
    sceneEditor * scene; 
    std::vector<ActionCell> cells;
public:
    void init(sceneEditor * scene);
    void addMessage(const char * n, float timemultiplier = 1.0f);
    bool pushGlobalEvent(GlobalEvent evt);
    void render();
    void clear();
    ~ActionLog();
};