#include "frame.hpp"
#include "error.hpp"

Layer::Layer(Frame * frame){
    parent = frame;
    canvas = SDL_CreateRGBSurface(0, parent->getWidth(), parent->getHeight(), 32, 0x000000FF , 0x0000FF00, 0x00FF0000, 0xFF000000);

    glGenTextures(1, &canvasTex);
    glBindTexture(GL_TEXTURE_2D, canvasTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, parent->getWidth(), parent->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Layer::~Layer(){
    SDL_FreeSurface(canvas);
    glDeleteTextures(1, &canvasTex);
}

void Layer::bindTexture(){
    if(parent->isLoaded()){
        glBindTexture(GL_TEXTURE_2D, canvasTex);
        if(needUpdate){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, parent->getWidth(), parent->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas->pixels);
            needUpdate=false;
        }
    }
}

void Layer::setVisibility(bool n){
    visible=n;
}

bool Layer::isVisible(){
    return visible;
}

SDL_Surface * Layer::getCanvas(){
    return canvas;
}

void Layer::forceUpdate(){
	needUpdate=true;
}

void Frame::removeLayer(int n){
    if(getLayerCount()>1){
        delete layers[n];
        layers.erase(layers.begin()+n);
    }
    if(getSelectedLayerIndex()>=getLayerCount()){
        selectLayer(getLayerCount()-1);
    }
}

int Frame::createLayer(){
    layers.push_back(new Layer(this));
    return layers.size()-1;
}

void Frame::selectLayer(int n){
    int c;
    if(n<0){
        c=0;
    }
    else if(n>=getLayerCount()){
        c=getLayerCount()-1;
    }
    else{
        c=n;
    }
    selectedLayerIndex=c;
}

Layer * Frame::getSelectedLayer(){
    return layers[selectedLayerIndex];
}

int Frame::getSelectedLayerIndex(){
    return selectedLayerIndex;
}

Layer * Frame::getLayer(int n){
    int c;
    if(n<0){
        c=0;
    }
    else if(n>=getLayerCount()){
        c=getLayerCount()-1;
    }
    else{
        c=n;
    }
    return layers[c];
}

int Frame::getLayerCount(){
    return layers.size();
}


Frame::Frame(){
    initvars();
}
Frame::~Frame(){
    free();
}

void Frame::free(){
    if(loaded){
        loaded=false;
        for(uint i=0; i<layers.size();i++){
            delete layers[i];
        }
        layers.clear();
    }
}

void Frame::initvars(){
    loaded=false;
    width=0;
    height=0;
    index=-1;
}


void Frame::load(int w, int h){
    if(loaded){ 
       free();
    }
    selectedLayerIndex=0;
    width=w;
    height=h;
    createLayer();
    
    loaded=true;
}


bool Frame::isLoaded(){
    return loaded;
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
