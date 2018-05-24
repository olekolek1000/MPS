#include "rectangle.hpp"
#include "../../drawer.hpp"
#include "../toolbox.hpp"

#include <sstream>

void Rectangle::init(){
    iconName = "rectangle";
}

void Rectangle::eventMouseMove(int x, int y){
    if(!down){
        startX=x;
        startY=y;
    }
    currentX=x;
    currentY=y;
    draw();
}

void Rectangle::eventMouseDown(){
    down=true;
    draw();
}

void Rectangle::eventMouseUp(){
    if(down){
        down=false;
        drawer->activeBlit();
    }
    drawer->historyCreateSnapshot();
}

void Rectangle::draw(){
    if(down){
        drawer->activeClear();
        drawer->activeDrawRectangle(startX, startY, currentX-startX, currentY-startY);
    }
}

void Rectangle::update(){
   
}

bool Rectangle::pushEvent(SDL_Event * evt){
    bool used=false;

    return used;
}


void Rectangle::render(){
    
}

void Rectangle::select(){
    
}
