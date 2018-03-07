#include "line.hpp"
#include "../../drawer.hpp"
#include "../toolbox.hpp"
#include <sstream>

void Line::init(){
    iconName = "line";
    box.init(scene).setMinValue(1).setMaxValue(512);
}

void Line::eventMouseMove(int x, int y){
    this->x=x;
    this->y=y;
    if(down){
        moved=true;
    }
}

void Line::eventMouseDown(){
    startX=x;
    startY=y;
    down=true;
}

void Line::eventMouseUp(){
    down=false;
    drawer->activeBlit();
    drawer->historyCreateSnapshot();
}

bool Line::pushEvent(SDL_Event * evt){
    bool used=false;

    if(box.pushEvent(evt)){
		used=true;
	}

    return used;
}


void Line::update(){
    box.update();
}
void Line::render(){
    if(down&&moved){
        drawer->activeClear();
        drawer->activeDrawLine(startX,startY,x,y,box.getValue());
        moved=false;
    }

    box.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()).setSize(toolbox->getWidth(),75);
    box.render();
}
void Line::select(){
    toolbox->setSettingsHeight(75);
}
