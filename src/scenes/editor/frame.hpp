#pragma once

#include "lib/sdl.hpp"
#include "lib/opengl.hpp"

#include <vector>

typedef unsigned int uint;

class Frame;

class Layer{
private:
    friend class Frame;
    Frame * parent;
    SDL_Surface * canvas = NULL;
    uint canvasTex;
    bool visible = true;
public:
    bool needUpdate = false;
    void bindTexture();
   
    SDL_Surface * getCanvas();
    void forceUpdate();
    bool isVisible();
    void setVisibility(bool n);
    Layer(Frame * frame);
    ~Layer();
};

class Frame{
private:
    friend class Layer;
    friend class Drawer;
    friend class FrameManager;

    std::vector<Layer*> layers;

    int width;
    int height;
    int index;
    bool loaded;

    int selectedLayerIndex = 0;

    void initvars();
public:
    void selectLayer(int n);
    Layer * getSelectedLayer();
    int getSelectedLayerIndex();
    Layer * getLayer(int n);
    int getLayerCount();
    int createLayer();
    void resize(int width, int height);
    void removeLayer(int n);

    Frame();
    ~Frame();
    void free();
    void load(int w, int h);
    bool isLoaded();
    int getWidth();
    int getHeight();
    float getProportions();
    int getIndex();
};
