#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>

#include "menu.hpp"
#include "../editor.hpp"
#include "app.hpp"
#include "timestep.hpp"
#include "lib/sdl_image.hpp"
#include "lib/floats.hpp"
#include "transform.hpp"
#include "render/func.hpp"
#include "error.hpp"

#include "tinyfiledialogs.hpp"
#include "gif.hpp" 

Menu::Menu(sceneEditor * scene){
	this->scene = scene;
	this->a = &scene->a; 
}

Menu::~Menu(){
	
}

void Menu::setProjection(){
	projection = glm::ortho(0.0f, (float)a->getAreaWidth(), (float)a->getAreaHeight(), 0.0f);

	for(auto iter = buttons.begin(); iter!=buttons.end(); iter++){
		iter->second.updateObjPos();
	}
}

void Menu::exitMenu(){
	x_target=-0.757;
	y_target=0.0;
	z_target=0.0;
	animspeed=0.4;
	exit_delay=12;
}


const int star_lifetime = 150;
class Star{
public:
	bool active=true;
	float posX,posY,posZ,velX,velY,velZ,posX_prev,posY_prev,posZ_prev;
	int time=0;

	void init(){
		velX = ((rand()%1000)/1000.0-0.5)/600.0;
		velY = ((rand()%1000)/1000.0)/600.0;
		velZ = ((rand()%1000)/1000.0-0.5)/600.0;
		posX = -(rand()%1000)/1000.0;
		posY = (rand()%1000)/1000.0-0.5;
		posZ = ((rand()%1000)/1000.0-0.5)*1.5;
		posX_prev=posX;
		posY_prev=posY;
		posZ_prev=posZ;
	}
	void update(){
		time++;
		if(time>star_lifetime){
			active=false;
		}
		posX_prev=posX;
		posY_prev=posY;
		posZ_prev=posZ;
		posX=posX+velX;
		posY=posY+velY;
		posZ=posZ+velZ;
		velY=velY-0.000025;
	}
};

void Menu::loop(){
	SDL_Event evt;
	
	setProjection();
	
	int width = a->getWindowWidth();
	int height = a->getWindowHeight();
	
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
	scene->thMan.addTexture("menu/menubutton","menu/menubutton.png",TEXSPACE_RGBA,TEXFILTERING_LINEAR);
	
	/*sides
	A===B===C
	|.......|
	H...I...D
	|.......|
	G===F===E
	*/
	buttons["01back"].init(this, 216, 66, 'A', "Go back");
	buttons["loadproject"].init(this, 216, -150, 'D', "Load project");
	buttons["saveproject"].init(this, 216, -50, 'D', "Save project");
	buttons["import"].init(this, 216, 50, 'D', "Import");
	buttons["export"].init(this, 216, 150, 'D', "Export");
	buttons["99quit"].init(this, 216, 66, 'G', "Exit");

	float postarget = 1.0;

	vector<Star> stars;
	for(int i=0;i<800;i++){
		stars.push_back(Star());
		stars.back().init();
	}
	vector<Float4> stars_points;//X, Y, Z, intensity
	RBuffer buf_stars;

	step.reset();
	step.setRate(30);
	
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
				case SDL_MOUSEMOTION:{
					if(exit_delay==0){
						z_target = -(evt.motion.x*scene->a.getAreaMultipler()-scene->a.getAreaWidth()/2)/(float)scene->a.getAreaWidth()*0.2;
						y_target = (evt.motion.y*scene->a.getAreaMultipler()-scene->a.getAreaHeight()/2)/(float)scene->a.getAreaHeight()*0.2;
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
				}
			}
			
			if(buttons["01back"].isClicked()){
				actionBack();
			}
			if(buttons["loadproject"].isClicked()){
				actionLoadProject();
			}
			if(buttons["saveproject"].isClicked()){
				actionSaveProject();
			}
			if(buttons["import"].isClicked()) {
				actionImport();
			}
			if(buttons["export"].isClicked()){
				actionExport();
			}
			if(buttons["99quit"].isClicked()){
				actionQuit();
			}

			for(int i=0; i<15; i++){
				stars.push_back(Star());
				stars.back().init();
			}

			for(int i = (int)stars.size()-1; i>=0; i--){
				stars[i].update();
				if(stars[i].active==false){
					stars.erase(stars.begin()+i);
				}
			}
		}

		{//render
			scene->shGui.select();scene->shGui.setP(&projection);
		
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
				a->square_vert->draw(GL_TRIANGLES);
			}
			{//3d screen overlay
				buf_plane.bind().attrib(0,3,GL_FLOAT);
				a->square_uv->bind().attrib(1,2,GL_FLOAT);
				
				tex.select();
				zReset(&tModel);
				zScale(&tModel,1.0,1.0,(float)width/(float)height);
				
				sh3d.select().setP(&tProjection).setV(&tView).setM(&tModel);
				buf_plane.draw(GL_TRIANGLES);
				
				sh3dref.select().setP(&tProjection).setV(&tView).setM(&tModel).setTime(SDL_GetTicks());
				buf_plane.draw(GL_TRIANGLES);
			}
			{//stars
				stars_points.resize(stars.size());
				for(uint i=0; i<stars.size(); i++){
					stars_points[i].x = alphize(step.getAlpha(),stars[i].posX_prev,stars[i].posX);
					stars_points[i].y = alphize(step.getAlpha(),stars[i].posY_prev,stars[i].posY);
					stars_points[i].z = alphize(step.getAlpha(),stars[i].posZ_prev,stars[i].posZ);
					stars_points[i].w = 1.0 - stars[i].time/(float)star_lifetime;
				}

				shStars.select().setP(&tProjection).setV(&tView);
				buf_stars.bind().setData(stars_points.size()*sizeof(Float4), stars_points.data(), GL_DYNAMIC_DRAW).attrib(0, 4, GL_FLOAT).draw(GL_POINTS);

			}
			if(exit_delay==0&&end==false)
			{//buttons
				for(auto iter = buttons.begin(); iter!=buttons.end(); iter++){
					iter->second.render(step.getAlpha());
				}
			}
		}
		a->updateAll();
	}
	scene->thMan.removeTexture("menu/background");
	scene->thMan.removeTexture("menu/menubutton");
	buttons.clear();
}

void Menu::dialogPrepare(){
	if(scene->a.isFullscreen()){
		wasFullscreen=true;
		scene->a.setFullscreen(false);
	}
	else{
		wasFullscreen=false;
	}
	SDL_HideWindow(a->window);
}

void Menu::dialogEnd(){
	if(wasFullscreen){
		scene->a.setFullscreen(true);
	}
	step.reset();
	SDL_ShowWindow(a->window);
}


void Menu::actionBack(){
	exitMenu();
}

void Menu::actionExport(){
	dialogPrepare();
	bool end=false;
	while(!end){
		end=true;
		const char * patterns[2] = { "*.gif", "*.png" };
		const char * name = tinyfd_saveFileDialog("Export animation...", NULL, 2, patterns, NULL);
		if(name!=NULL){
			string str_name = string(name);
			if(str_name.size()>4){
				string str_ext = str_name.substr( str_name.length() - 4 );
				if(str_ext==".gif"){
					int delay = 15;
					int width = scene->frameMan.getFrame(0)->getWidth();
					int height = scene->frameMan.getFrame(0)->getHeight();
					
					GifWriter writer;
					GifBegin(&writer, name, width, height, delay, 8, false);
					for(int i=0; i<scene->frameMan.getFrameCount(); i++){
						SDL_Surface * overlay = scene->frameMan.getFrame(i)->getOverlay();
						GifWriteFrame(&writer, (const uint8_t*)overlay->pixels, overlay->w, overlay->h, delay, 8, false);
					}
					GifEnd(&writer);
				}
				else{
					error("Invalid file extension");
					end=false;
				}
			}
			else{
				error("Invalid filename");
				end=false;
			}
		}
	}
	dialogEnd();
}


void Menu::actionImport(){
	dialogPrepare();

	error("Not yet implemented! Sorry.");

	dialogEnd();
}

void Menu::actionLoadProject(){
	dialogPrepare();
	const char * patterns[1] = {"*.mpsp"};
	const char * name = tinyfd_openFileDialog("Load project...", NULL, 1, patterns, "Moving Picture Studio Project (*.mpsp)", false);
	if(name!=NULL){
		std::fstream file;
		file.open(name, std::ios::in|std::ios::binary);
		if(file.good()){
			string pref;
			pref.resize(strlen(filePrefix));
			file.read((char*)pref.c_str(), strlen(filePrefix));
			
			if(pref!=string(filePrefix)){
				error("This is not a MPS project!");
			}
			else{
				int fileVersion;
				file.read((char*)&fileVersion, sizeof(int));
				
				switch(fileVersion){
					case 1:{
						int frameCount;
						file.read((char*)&frameCount, sizeof(int));
						
						scene->frameMan.clearFrames();
						//frames
						for(int i=0; i<frameCount; i++){
							int width;
							file.read((char*)&width, sizeof(int));//width
							int height;
							file.read((char*)&height, sizeof(int));//height
							scene->frameMan.selectFrame(scene->frameMan.getFrameCount());
							scene->frameMan.createFrame(width, height);
							scene->frameMan.getFrame(i)->forceUpdate();
							file.read((char*)scene->frameMan.getFrame(i)->getOverlay()->pixels, width*height*4);//pixels
						}
						scene->changeFrame(0);
						exitMenu();
						break;
					}
					default:{
						error("File version not supported. Upgrade your program.");
					}
				}
			}
			
			file.close();
		}
		else{
			error("Cannot load file! Check your filename and location.");
		}
	}
	dialogEnd();
}

void Menu::actionSaveProject(){
	dialogPrepare();
	const char * patterns[1] = {"*.mpsp"};
	const char * name = tinyfd_saveFileDialog("Save project...", NULL, 1, patterns, "Moving Picture Studio Project (*.mpsp)");
	if(name!=NULL){
		std::fstream file;
		file.open(name, std::ios::out|std::ios::binary);
		if(file.good()){
			int frameCount = scene->frameMan.getFrameCount();
			
			//header
			file.write((char*)filePrefix, strlen(filePrefix));
			file.write((char*)&fileVersion, sizeof(int));
			file.write((char*)&frameCount, sizeof(int));
			
			//frames
			for(int i=0; i<frameCount; i++){
				Frame * frame = scene->frameMan.getFrame(i);
				int width = frame->getWidth();
				int height = frame->getHeight();
				file.write((char*)&width, sizeof(int));//width
				file.write((char*)&height, sizeof(int));//height
				file.write((char*)frame->getOverlay()->pixels, width*height*4);//pixels
			}
			
			file.close();
		}
		else{
			error("Cannot create file! Check your permissions or location.");
		}
	}
	dialogEnd();
}
void Menu::actionQuit(){
	exit(0);
}