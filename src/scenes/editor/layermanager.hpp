#pragma once

#include "frame.hpp"
#include "gui/button.hpp"
#include "gui/text.hpp"
#include <vector>

class sceneEditor;

class LayerCell{
    friend class LayerManager;
    GuiButton but_set;
    GuiButton but_visibility;
    GuiButton but_remove;
    GuiText text_index;
    Layer * layer;
public:
    void init(Layer * layer);
    ~LayerCell();
};

class LayerManager{
    sceneEditor * scene;
    Frame * currentFrame = NULL;
    GuiButton but_addLayer;
    GuiText text_buildinprogress;
    void updateList();
    std::vector<LayerCell> cells;
public:
    void setCurrentFrame(Frame * frame);
    void init(sceneEditor * scene);
    bool pushEvent(SDL_Event * evt);
    void update();
    void render(float alpha);
};