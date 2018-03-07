#include "valuebox.hpp"
#include "../editor.hpp"
#include "scenes/editor/gui/button.hpp"
#include "scenes/editor/gui/text.hpp"

GuiValueBox::GuiValueBox(){
	but_up = new GuiButton;
	but_down = new GuiButton;
	text_value = new GuiText;
}
GuiValueBox::~GuiValueBox(){
	delete but_up;
	delete but_down;
	delete text_value;
}

GuiValueBox& GuiValueBox::init(sceneEditor* scene){
	this->scene = scene;
	initInterface();
	updateValue();
	return *this;
}

GuiValueBox& GuiValueBox::init(sceneEditor* scene, int posX, int posY, int width, int height){
	this->scene = scene;
	this->posX=posX;
	this->posY=posY;
	this->width=width;
	this->height=height;
	initInterface();
	updateSize();
	updateValue();
	return *this;
}

GuiValueBox& GuiValueBox::setPosition(int posX, int posY){
	this->posX=posX;
	this->posY=posY;
	updateSize();
	return *this;
}

GuiValueBox& GuiValueBox::setSize(int width, int height){
	this->width=width;
	this->height=height;
	updateSize();
	return *this;
}

int GuiValueBox::getWidth(){
	return width;
}

int GuiValueBox::getHeight(){
	return height;
}

int GuiValueBox::getPosX(){
	return posX;
}

int GuiValueBox::getPosY(){
	return posY;
}

GuiValueBox& GuiValueBox::update(){
	if(but_down->isPressed()||but_up->isPressed()){
        holdtime++;
        if(holdtime>8){
            if(but_down->isPressed()){
                value-=(holdtime-5)/8+1;
                if(value<minvalue)value=minvalue;
				updateValue();
            }
            else if(but_up->isPressed()){
                value+=(holdtime-5)/8+1;
                if(value>maxvalue)value=maxvalue;
				updateValue();
            }
        }
    }
    else{
        holdtime=0;
    }
	return *this;
}

GuiValueBox& GuiValueBox::render(){
	if(needupdatesize){
		but_up->setPosition(posX,posY).setSize(height/2,height/2);
		but_up->setTexture(&scene->thMan["icons/up"]);
		
		but_down->setPosition(posX,posY+height/2).setSize(height/2,height/2);
		but_down->setTexture(&scene->thMan["icons/down"]);

		text_value->setPosition(posX+width/2+height/4,posY+height/2);
		text_value->setAlign(1,1);
		
		needupdatesize=false;
	}
	
	but_down->render();
	but_up->render();
	
	if(valuechanged){
		text_value->changeText(std::to_string(value),scene->a.font32,255,255,255);
		valuechanged=false;
	}
	text_value->render();
	
	return *this;
}

bool GuiValueBox::pushEvent(SDL_Event* evt){
	bool used=false;
	
	if(but_up->pushEvent(evt)){
        if(but_up->isClicked()){
            value++;
            if(value>maxvalue)value=maxvalue;
			updateValue();
        }
        used=true;
    }
    if(but_down->pushEvent(evt)){
        if(but_down->isClicked()){
			value--;
			if(value<minvalue)value=minvalue;
			updateValue();
        }
        used=true;
    }
	
	return used;
}

void GuiValueBox::initInterface(){
	but_down->init(scene);
	but_up->init(scene);
	text_value->init(scene);
}

void GuiValueBox::updateSize(){
	needupdatesize=true;
}

GuiValueBox& GuiValueBox::setMinValue(int n){
	minvalue=n;
	return *this;
}

GuiValueBox& GuiValueBox::setMaxValue(int n){
	maxvalue=n;
	return *this;
}

int GuiValueBox::getMinValue(){
	return minvalue;
}

int GuiValueBox::getMaxValue(){
	return maxvalue;
}

int GuiValueBox::getValue(){
	return value;
}

GuiValueBox& GuiValueBox::setValue(int n){
	value = n;
	updateValue();
	return *this;
}

void GuiValueBox::updateValue(){
	valuechanged=true;
}