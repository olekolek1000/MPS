#include "frame.h"

Frame::Frame(){
    initvars();
}

void Frame::initvars(){
    loaded=false;
    needUpdate=false;
    width=0;
    height=0;
    index=-1;
}

void Frame::free(){
    if(loaded){
        SDL_FreeSurface(overlay);
        glDeleteTextures(1, &overlayTex);
        loaded=false;
    }
}
void Frame::load(int w, int h){
    if(loaded){ 
        free();
    }

    width=w;
    height=h;

    overlay = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF , 0x0000FF00, 0x00FF0000, 0xFF000000);

    glGenTextures(1, &overlayTex);
    glBindTexture(GL_TEXTURE_2D, overlayTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, overlay->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    loaded=true;
}
Frame::~Frame(){
    free();
}

bool Frame::isLoaded(){
    return loaded;
}

void Frame::bindTexture(){
    if(loaded){
        glBindTexture(GL_TEXTURE_2D, overlayTex);
        if(needUpdate){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, overlay->pixels);
            needUpdate=false;
        }
    }
}

int Frame::getWidth(){
    return width;
}

int Frame::getHeight(){
    return height;
}

float Frame::getProportions(){
    return (float)width/(float)height;
}

int Frame::getIndex(){
    return index;
}

SDL_Surface * Frame::getOverlay(){
    return overlay;
}


void Frame::forceUpdate(){
	needUpdate=true;
}