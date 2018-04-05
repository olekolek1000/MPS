#pragma once

#include "frame.hpp"
#include "lib/glm.hpp"
#include "lib/floats.hpp"
#include "render/buffer.hpp"

#include <vector>

class Tool;
class Toolbox;
class sceneEditor;


class HistoryCell{
private:
    int width;
    int height;
    bool loaded=false;
    char * raw;
    int rawsize;
public:
    int getRawSize();
    void create(SDL_Surface * surf);
    char * getRaw();
    void free();
    ~HistoryCell();
    int getWidth();
    int getHeight();
};



class Drawer{
private:
    Frame * currentFrame = NULL;
    Toolbox * toolbox;
    Uint8 r=0,g=0,b=0;
    int color = 0xFF000000;//black
    sceneEditor * scene;
    glm::mat4 projection, model;

    bool dragging=false;
	bool rotating=false;
    Float2 dragCameraStart={0.0,0.0};
    Float2 dragStart={0.0,0.0};
    Float2 dragEnd={0.0,0.0};

    float cameraX;
    float cameraY;
    float cameraZoom;
    float cameraZoomSmooth;
    float cameraZoomSmoothPrev; 
    float cameraZoomAlpha;
	
	RBuffer bordercoordbuf;
	RBuffer bordercolorbuf;
	RBuffer linebuf;
	void updateBorders();
	
	Float2 cameraRot={0,1},cameraRotSmooth={0,1},cameraRotSmoothPrev={0,1},cameraRotAlpha={0,1},rotationStart={0,1};

    int ghostback = 4;
    int ghostfront = 4;
    float ghostopacity = 0.2;

    int drawX=0,drawY=0;

    int mouseX=0,mouseY=0;

    SDL_Surface * activeOverlay = NULL;
    uint activeOverlayTex;
    bool activeNeedUpdate=true;
    float activealpha=1.0;
	
	float animationSpeed;
	float animationSpeedDefault=0.5;

    std::vector <HistoryCell*> history;
    int historypos=0; 
    int historylimit=50;
public:
    void init(sceneEditor * scene);
    ~Drawer();

    void historyCreateSnapshot();
    void historyUndo();
    void historyRedo();
    void historyClear();
    void historyUpdate();

    void activeClear();
    void activeBlit(float alpha = 1.0);
    void activePreviewAlpha(float alpha);

    void setCurrentFrame(Frame * frame);
    void setColor(Uint8 r, Uint8 g, Uint8 b);
    int getSelectedColor();

    void drawPoint(int x, int y);
    void drawRectangle(int x, int y, int w, int h);
    void drawLine(int startX, int startY, int endX, int endY, int thickness);

    void activeDrawPoint(int x, int y);
    void activeDrawRectangle(int x, int y, int w, int h);
    void activeDrawLine(int startX, int startY, int endX, int endY, int thickness);

    int getPoint(int x, int y);

    void updateViewport();

    void setCameraPosition(float x, float y);
    Float2 getCameraPosition();
    void zoomIn(float n);
    void zoomOut(float n);
    void setZoomPixelPerfect(int n);

    void update();
    void render(float alpha);
	
	void reloadTextures();

    bool pushEvent(SDL_Event * evt);
};
