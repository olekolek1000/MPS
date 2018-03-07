#include "menubutton.hpp"
#include "menu.hpp"
#include "../editor.hpp"
#include "transform.hpp"
#include "render/func.hpp"
#include "timestep.hpp"

void MenuButton::init(Menu * menu, int posY, std::string text){
	this->menu = menu;
	this->scene = menu->scene;
	this->posX = -400;
	this->posX_prev = posX;
	this->posX_smooth = posX;
	this->posY = posY;
	this->width = 400;
	this->height = 96;
	
	this->text.init(scene);
	this->text.changeText(text, scene->a.font64, 255, 255, 255);
	this->text.setAlign(1,1);
	
	this->scale = 1.0;
	this->scale_prev = this->scale;
	this->scale_target = this->scale;
}


void MenuButton::setXPosition(int posX){
	this->posX = posX;
}

void MenuButton::update(){
	scale_prev = scale;
	scale = scale + (scale_target - scale)*0.5;
	posX_prev=posX;
	
	if(hovered){
		scale_target = 1.1;
	}
	else{
		scale_target = 1.0;
	}
}

void MenuButton::render(float alpha){
	posX_smooth = alphize(alpha, posX_prev, posX);
	xReset(&model);
	xTranslate(&model, posX_smooth, posY);
	float sc = alphize(alpha, scale_prev, scale);
	xScale(&model, width, height);
	xTranslate(&model, 0.5, 0.5);
	xScale(&model, sc, sc);
	xRotate(&model, (1.0-sc)/3.0);
	xTranslate(&model, -0.5, -0.5);
	scene->thMan["menu/menubutton"].select();
	scene->shGui.select();
	scene->shGui.setM(&model);
	scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
	scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
	scene->a.square_vert->draw(GL_TRIANGLES);
	
	this->text.setPosition(posX_smooth+width/2, posY+height/2);
	text.render();
}

bool MenuButton::pushEvent(SDL_Event * evt){
	bool used=false;
	
	switch(evt->type){
        case SDL_MOUSEBUTTONDOWN:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(evt->button.x>=posX&&evt->button.y>=posY&&evt->button.x<posX+width&&evt->button.y<posY+height){
                    used=true;
                    pressed=true;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(pressed){
                    if(evt->button.x>=posX&&evt->button.y>=posY&&evt->button.x<posX+width&&evt->button.y<posY+height){
                        used=true;
                        clicked=true;
                    }
                    pressed=false;
                }
            }

            break;
        }
        case SDL_MOUSEMOTION:{
			if(evt->motion.x>=posX&&evt->motion.y>=posY&&evt->motion.x<posX+width&&evt->motion.y<posY+height){
				hovered=true;
			}
			else{
				hovered=false;
			}
            if(pressed){
                used=true;
            }
            break;
        }
    }
	
	return used;
}

bool MenuButton::isClicked(){
	if(clicked){
        clicked=false;
        return true;
    }
    else{
        return false;
    }
}

bool MenuButton::isPressed(){
	return pressed;
}