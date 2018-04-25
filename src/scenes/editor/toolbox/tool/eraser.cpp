#include "eraser.hpp"

#include "../../drawer.hpp"
#include "../toolbox.hpp"

#include <sstream>

void Eraser::init(){
    iconName = "eraser";

    box.init(scene).setMinValue(1).setMaxValue(2048);
}

void Eraser::eventMouseMove(int x, int y){
    prevX=this->x;
    prevY=this->y;
    this->x=x;
    this->y=y;
    draw();
}

void Eraser::eventMouseDown(){
    down=true;
    draw();
}

void Eraser::eventMouseUp(){
    if(down){
        down=false;
    }
    drawer->historyCreateSnapshot();
}

void Eraser::draw(){
    if(down){
        drawer->setErase(true);
        drawer->drawLine(prevX,prevY,x,y,box.getValue());
        drawer->setErase(false);
    }
}

void Eraser::update(){
    box.update();
}

bool Eraser::pushEvent(SDL_Event * evt){
    bool used=false;

    if(box.pushEvent(evt)){
		used=true;
	}

    return used;
}


void Eraser::render(){
    drawer->setBrushCircleSize(box.getValue());
	box.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()).setSize(toolbox->getWidth(),75);
    box.render();
}

void Eraser::select(){
    toolbox->setSettingsHeight(75);
}
