#include "pencil.hpp"
#include "../../drawer.hpp"
#include "../toolbox.hpp"

void Pencil::init(){
    iconName = "pencil";

    box.init(scene).setMinValue(1).setMaxValue(2048);
}

void Pencil::eventMouseMove(int x, int y){
    prevX=this->x;
    prevY=this->y;
    this->x=x;
    this->y=y;
    draw();
}

void Pencil::eventMouseDown(){
    down=true;
    //scene->drawer.activePreviewAlpha(0.3);
    draw();
}

void Pencil::eventMouseUp(){
    if(down){
        down=false;
        drawer->activeBlit();
    }
    drawer->historyCreateSnapshot();
}

void Pencil::draw(){
    if(down){
        drawer->activeDrawLine(prevX,prevY,x,y,box.getValue());
    }
}

void Pencil::update(){
    box.update();
}

bool Pencil::pushEvent(SDL_Event * evt){
    bool used=false;

    if(box.pushEvent(evt)){
		used=true;
	}

    return used;
}


void Pencil::render(float alpha){
    drawer->setBrushCircleSize(box.getValue());
	box.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()).setSize(toolbox->getWidth(),75);
    box.render(alpha);
}

void Pencil::select(){
    toolbox->setSettingsHeight(75);
}
