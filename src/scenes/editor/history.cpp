#include "drawer.h"
#include "error.h"

void HistoryCell::create(SDL_Surface * surf){
    free();

    width=surf->w;
    height=surf->h;

    rawsize = width*height*4;

    raw = new char[rawsize];
    memcpy(raw, surf->pixels, rawsize);

    loaded=true;
}

int HistoryCell::getRawSize(){
    return rawsize;
}

char * HistoryCell::getRaw(){
    return raw;
}

void HistoryCell::free(){
    if(loaded){
        delete[]raw;
        loaded=false;
    }
}

HistoryCell::~HistoryCell(){
    free();
}

int HistoryCell::getWidth(){
    return width;
}

int HistoryCell::getHeight(){
    return height;
}
