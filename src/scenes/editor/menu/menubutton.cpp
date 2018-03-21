#include "menubutton.hpp"
#include "menu.hpp"
#include "../editor.hpp"
#include "transform.hpp"
#include "render/func.hpp"
#include "timestep.hpp"

void MenuButton::init(Menu * menu, int posX, int posY, char side, std::string text){
	this->menu = menu;
	this->scene = menu->scene;
	this->posX = posX;
	this->posX_prev = posX;
	this->posX_smooth = posX;

	this->posY = posY;
	this->posY_prev = posY;
	this->posY_smooth = posY;

	this->side = side;
	
	this->width = 400;
	this->height = 96;
	
	this->text.init(scene);
	this->text.changeText(text, scene->a.font64, 255, 255, 255);
	this->text.setAlign(1,1);
	
	this->scale = 1.0;
	this->scale_prev = this->scale;
	this->scale_target = this->scale;

	updateObjPos();
}

void MenuButton::setPosition(int posX, int posY){
	this->posX = posX;
	this->posY = posY;
	updateObjPos();
}

void MenuButton::setXPosition(int posX){
	this->posX = posX;
	updateObjPos();
}

void MenuButton::setYPosition(int posY){
	this->posY = posY;
	updateObjPos();
}

void MenuButton::update(){
	scale_prev = scale;
	scale = scale + (scale_target - scale)*scale_speed;
	posX_prev=posX;
	posY_prev=posY;
	
	if(hovered){
		scale_target = 1.1;
		scale_speed = 0.8;
	}
	else{
		scale_target = 1.0;
		scale_speed = 0.2;
	}
}

void MenuButton::updateObjPos(){
	int areaW = scene->a.getAreaWidth();
	int areaH = scene->a.getAreaHeight();
	switch(side){
		case 'A':{
			objX = posX_smooth;
			objY = posY_smooth;
			break;
		}
		case 'B':{
			objX = areaW/2+posX_smooth;
			objY = posY_smooth;
			break;
		}
		case 'C':{
			objX = areaW-posX_smooth;
			objY = posY_smooth;
			break;
		}
		case 'D':{
			objX = areaW-posX_smooth;
			objY = areaH/2+posY_smooth;
			break;
		}
		case 'E':{
			objX = areaW-posX_smooth;
			objY = areaH-posY_smooth;
			break;
		}
		case 'F':{
			objX = areaW/2+posX_smooth;
			objY = areaH-posY_smooth;
			break;
		}
		case 'G':{
			objX = posX_smooth;
			objY = areaH-posY_smooth;
			break;
		}
		case 'H':{
			objX = posX_smooth;
			objY = areaH/2+posY_smooth;
			break;
		}
		case 'I':{
			objX = areaW/2+posX_smooth;
			objY = areaH/2+posY_smooth;
			break;
		}
		default:{
			assert("Invalid button side");
		}
	}
}

void MenuButton::setSide(char side){
	this->side = side;
}

void MenuButton::render(float alpha){
	posX_smooth = alphize(alpha, posX_prev, posX);
	posY_smooth = alphize(alpha, posY_prev, posY);

	float sc = alphize(alpha, scale_prev, scale);
	xReset(&model);
	xTranslate(&model, objX-width/2, objY-height/2);
	xScale(&model, width, height);
	xTranslate(&model, 0.5, 0.5);
	xScale(&model, sc, sc);
	xTranslate(&model, -0.5, -0.5);
	scene->thMan["menu/menubutton"].select();
	scene->shGui.select();
	scene->shGui.setM(&model);
	scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
	scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
	scene->a.square_vert->draw(GL_TRIANGLES);
	
	this->text.setPosition(objX, objY);
	text.render();
}

bool MenuButton::pushEvent(SDL_Event * evt){
	bool used=false;
	
	switch(evt->type){
        case SDL_MOUSEBUTTONDOWN:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(evt->motion.x>=objX-width/2&&evt->motion.y>=objY-height/2&&evt->motion.x<objX+width/2&&evt->motion.y<objY+height/2){
                    used=true;
                    pressed=true;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(pressed){
                    if(evt->motion.x>=objX-width/2&&evt->motion.y>=objY-height/2&&evt->motion.x<objX+width/2&&evt->motion.y<objY+height/2){
                        used=true;
                        clicked=true;
                    }
                    pressed=false;
                }
            }

            break;
        }
        case SDL_MOUSEMOTION:{
			if(evt->motion.x>=objX-width/2&&evt->motion.y>=objY-height/2&&evt->motion.x<objX+width/2&&evt->motion.y<objY+height/2){
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