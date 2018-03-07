#include "spray.hpp"
#include "../../drawer.hpp"
#include "../toolbox.hpp"

void Spray::init(){
    iconName = "spray";
	
	box_size.init(scene).setMinValue(3).setMaxValue(2048).setValue(16);
	box_intensity.init(scene).setMinValue(1).setMaxValue(100).setValue(10);
}

void Spray::eventMouseMove(int x, int y){
    this->x=x;
    this->y=y;
    draw();
}

void Spray::eventMouseDown(){
    down=true;
    draw();
}

void Spray::eventMouseUp(){
    if(down){
        down=false;
        drawer->activeBlit();
    }
    drawer->historyCreateSnapshot();
}

void Spray::draw(){
    int noiseX,noiseY;
	int sz = box_size.getValue();
    if(down){
        for(int i=0;i<sz*(box_intensity.getValue()/10.0);i++){
            noiseX=rand()%sz-sz/2;
            noiseY=rand()%sz-sz/2;
            if(pow(noiseX,2)+pow(noiseY,2)<pow(sz/2,2)){
                drawer->activeDrawPoint(x+noiseX, y+noiseY);
            }
        }
    }
}

bool Spray::pushEvent(SDL_Event * evt){
    bool used=false;

	if(box_size.pushEvent(evt)){
		used=true;
	}
	if(box_intensity.pushEvent(evt)){
		used=true;
	}

    return used;
}
void Spray::update(){
	box_size.update();
	box_intensity.update();
}
void Spray::render(){
	box_intensity.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()).setSize(toolbox->getWidth(),45);
	box_intensity.render();
	
	box_size.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()+45).setSize(toolbox->getWidth(),75);
	box_size.render();
}
void Spray::select(){
    toolbox->setSettingsHeight(120);
}
