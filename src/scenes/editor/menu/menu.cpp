#include "menu.h"
#include <vector>

#include "../editor.h"
#include "app.h"
#include "timestep.h"
#include "lib/sdl_image.h"
#include "transform.h"
#include "render/func.h"
#include "menubutton.h"
#include "filedialog.h"

#include "lib/SDL2_rotozoom.h"

#include <sstream>
#include <iostream>
#include <map>

Menu::Menu(sceneEditor * scene){
	this->scene = scene;
	this->a = &scene->a;
}

Menu::~Menu(){
	
}

void Menu::setProjection(){
	projection = glm::ortho(0.0f, (float)a->getAreaWidth(), (float)a->getAreaHeight(), 0.0f);
}

void Menu::exitMenu(){
	x_target=-0.757;
	y_target=0.0;
	z_target=0.0;
	animspeed=0.4;
	exit_delay=12;
}

void Menu::loop(){
	SDL_Event evt;
	
	setProjection();
	
	int width = a->getAreaWidth();
	int height = a->getAreaHeight();
	
	std::vector<unsigned char> data(width*height*4);
	glReadBuffer(GL_BACK);
	glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,&data[0]);
	Texture tex;
	tex.create(TEXFILTERING_LINEAR,GL_CLAMP_TO_BORDER);
	tex.update(data.data(),width,height,TEXSPACE_RGBA);
	data.clear();

	
	float plane[] = {
		 0.0, -0.5, -0.5,
		 0.0, 0.5, -0.5,
		 0.0, 0.5, 0.5,
		 0.0, -0.5, -0.5,
		 0.0, -0.5, 0.5,
		 0.0, 0.5, 0.5
		};
	RBuffer buf_plane;
	buf_plane.bind().setData(sizeof(plane),plane,GL_STATIC_DRAW);
	
	Shader sh3d;
	sh3d.load("editor/menu/3d.vsh","editor/menu/3d.fsh").createUniform("PROP").setUniform("PROP",(float)width/(float)height);
	
	Shader sh3dref;
	sh3dref.load("editor/menu/3dref.vsh","editor/menu/3dref.fsh");
	
	Shader shStars;
	shStars.load("editor/menu/stars.vsh","editor/menu/stars.fsh");

	
	float x = -0.757;
	float x_prev = x;
	float y = 0.0;
	float y_prev = y;
	float z = 0.0;
	float z_prev = z;
	
	scene->thMan.addTexture("menu/background","menu/background.png",TEXSPACE_RGB,TEXFILTERING_LINEAR);
	scene->thMan.addTexture("menu/soon","menu/soon.png",TEXSPACE_RGBA,TEXFILTERING_LINEAR);
	scene->thMan.addTexture("menu/menubutton","menu/menubutton.png",TEXSPACE_RGBA,TEXFILTERING_LINEAR);
	
	Timestep step;
	step.setRate(30);
	std::map<std::string, MenuButton> buttons;
	buttons["01back"].init(this, 200, "Go back");
	buttons["export"].init(this, 300, "Export");
	buttons["99quit"].init(this, 400, "Quit");

	float postarget = 1.0;
	
	bool end=false;
	while(!end){
		while(a->eventHandle(&evt)){
			switch(evt.type){
			    case SDL_KEYDOWN:{
                    switch(evt.key.keysym.sym){
                        case SDLK_ESCAPE:{
							exitMenu();
							break;
						}
                    }
                    break;
			    }
			}
			
			if(exit_delay==0&&end==false){
				for(auto iter = buttons.begin(); iter!=buttons.end(); iter++){
					iter->second.pushEvent(&evt);
				}
			}
		}
		
		while(a->onEvent()){
			switch(a->getEvent()){
				case GlobalEvent::WindowSizeChange:{
					setProjection();
					break;
				}
				case GlobalEvent::Exit:{
				    exit(0);
					end=true;
					break;
				}
				default:{
					break;
				}
			}
		}

		while(step.onUpdate())
		{//update
			x_prev=x;
			y_prev=y;
			z_prev=z;
			
			x = x + (x_target-x)*animspeed;
			y = y + (y_target-y)*animspeed;
			z = z + (z_target-z)*animspeed;
		
			if(exit_delay>0){
				exit_delay--;
				if(exit_delay==0){
					end=true;
				}
			}

			postarget*=0.88;
			
			if(exit_delay==0&&end==false){
				for(auto iter = buttons.begin(); iter!=buttons.end(); iter++){
					iter->second.update();
					iter->second.setXPosition(-300+400*(1.0-postarget));
				}
			}
			
			if(buttons["01back"].isClicked()){
				exitMenu();
			}
			if(buttons["export"].isClicked()){
				fd_saveFile();
			}
			if(buttons["99quit"].isClicked()){
				exit(0);
			}
		}

		{//render
			glClearColor(0.0,0.0,0.0,1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glm::mat4 tProjection = glm::perspective(45.15f,a->getProportions(),0.1f,100.0f);
			glm::mat4 tView = glm::lookAt(glm::vec3(alphize(step.getAlpha(),x_prev,x),alphize(step.getAlpha(),y_prev,y),alphize(step.getAlpha(),z_prev,z)),glm::vec3(0,0,0),glm::vec3(0,1,0));
			glm::mat4 tModel;
		
			int mouseX,mouseY;
			SDL_GetMouseState(&mouseX,&mouseY);
			
			a->square_vert->bind().attrib(0,2,GL_FLOAT);
			a->square_uv->bind().attrib(1,2,GL_FLOAT);
			{//background
				xReset(&model);
				xScale(&model, a->getAreaWidth(), a->getAreaHeight());
				scene->thMan["menu/background"].select();
				a->shMan["default2d"].select().setM(&model).setP(&projection);
				rDraw(GL_TRIANGLES,a->square_vert->getSize());
			}
			{//stars
				xReset(&model);
				xScale(&model, a->getAreaWidth(),a->getAreaHeight()/2);
				shStars.select().setM(&model).setP(&projection).setTime(step.getTime()/8.0).setResolution(a->getAreaWidth()/5.0f,a->getAreaHeight()/5.0f);
				rDraw(GL_TRIANGLES,a->square_vert->getSize());
			}
			{//3d screen overlay
				buf_plane.bind().attrib(0,3,GL_FLOAT);
				a->square_uv->bind().attrib(1,2,GL_FLOAT);
				
				tex.select();
				zReset(&tModel);
				zScale(&tModel,1.0,1.0,(float)width/(float)height);
				
				sh3d.select().setP(&tProjection).setV(&tView).setM(&tModel);
				rDraw(GL_TRIANGLES,buf_plane.getSize());
				
				sh3dref.select().setP(&tProjection).setV(&tView).setM(&tModel).setTime(SDL_GetTicks());
				rDraw(GL_TRIANGLES,buf_plane.getSize());
			}
			if(exit_delay==0&&end==false)
			{//buttons
				for(auto iter = buttons.begin(); iter!=buttons.end(); iter++){
					iter->second.render(step.getAlpha());
				}
			}
			
			{//build in progress text
				a->square_vert->bind().attrib(0,2,GL_FLOAT);
				a->square_uv->bind().attrib(1,2,GL_FLOAT);
				xReset(&model);
				xTranslate(&model, a->getAreaWidth()/2-200+sin(step.getTime())*10,a->getAreaHeight()-80);
				xScale(&model, 400, 80);
				scene->thMan["menu/soon"].select();
				a->shMan["default2d"].select().setM(&model).setP(&projection);
				rDraw(GL_TRIANGLES,a->square_vert->getSize());
			}
		}

		a->updateAll();
	}
	scene->thMan.removeTexture("menu/background");
	scene->thMan.removeTexture("menu/soon");
	scene->thMan.removeTexture("menu/menubutton");
}