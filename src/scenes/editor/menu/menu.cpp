#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>

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
#include "pixel.hpp"

#include "export/gif.hpp"
#include "exportinfo.hpp"

#include "../version.hpp"
#include "../gui/text.hpp"


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
	exit_delay=exit_delay_start;
}


const int star_lifetime = 500;
class Star{
public:
	bool active=true;
	float posX,posY,posZ,velX,velY,velZ,posX_prev,posY_prev,posZ_prev;
	int time=0;

	void init(){
		velX = ((rand()%1000)/1000.0-0.5)/1200.0;
		velY = ((rand()%1000)/1000.0)/1200.0;
		velZ = ((rand()%1000)/1000.0-0.5)/1200.0;
		posX = (-(rand()%1000)/1000.0)*1.3;
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
		velY=velY+0.0000008;
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

	GuiText text;
	{
		std::stringstream ss;
		ss<<"[Moving Picture Studio ver. "<<VERSION_STRING<<", "<<__DATE__<<" "<<__TIME__<<"]";
		text.init(scene).changeText(ss.str(), scene->a.font24, 255, 255, 255).setAlign(1, 0);
	}
	
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
	scene->thMan.addTexture("menu/star","menu/star.png",TEXSPACE_RGBA,TEXFILTERING_LINEAR);
	
	/*sides
	A===B===C
	|.......|
	H...I...D
	|.......|
	G===F===E
	*/
	buttons["back"].init(this, 216, 66, 'A', "Go back");
	buttons["loadproject"].init(this, 216, -150, 'D', "Load project");
	buttons["saveproject"].init(this, 216, -50, 'D', "Save project");
	buttons["import"].init(this, 216, 50, 'D', "Import");
	buttons["export"].init(this, 216, 150, 'D', "Export");
	buttons["exit"].init(this, 216, 66, 'G', "Exit");

	float postarget = 1.0;

	list<Star> stars;
	vector<float> stars_data;
	vector<Float3> stars_positions;
	vector<Float2> stars_uv;
	RBuffer buf_stars_data;
	RBuffer buf_stars_positions;
	RBuffer buf_stars_uv;

	step.reset();
	step.setRate(30);
	step.setSpeed(scene->a.config.getvarF("animationSpeed"));

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

		while(step.onUpdate()){//update
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
			
			if(buttons["back"].isClicked()){
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
			if(buttons["exit"].isClicked()){
				actionExit();
			} 

			for(int i=0; i<1; i++){
				stars.push_back(Star());
				stars.back().init();
			}
 
			for(auto i = stars.begin(); i != stars.end(); i++){
				i->update();
				if(i->active==false){
					i = stars.erase(i);
				}
			}
		}

		if(!scene->a.isHidden())
		{//render
			scene->shGui.select();scene->shGui.setP(&projection);
		
			glClearColor(0.0,0.0,0.0,1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glm::mat4 tProjection = glm::perspective(45.15f,a->getProportions(),0.005f,100.0f);
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
				stars_data.resize(stars.size()*6);
				stars_positions.resize(stars.size()*6);
				stars_uv.resize(stars.size()*6);
				int loop=0;
				float x,y,z,intensity,size;
				for (auto i = stars.begin(); i != stars.end(); i++){
					float intfab = 1.0-fabs(1.0-i->time/(float)star_lifetime*2);//intensity

					size = 0.01;

					x=alphize(step.getAlpha(),i->posX_prev,i->posX);
					y=alphize(step.getAlpha(),i->posY_prev,i->posY);
					z=alphize(step.getAlpha(),i->posZ_prev,i->posZ);

					stars_positions[loop*6 +0] = {x,y-size,z-size};
					stars_positions[loop*6 +1] = {x,y+size,z-size};
					stars_positions[loop*6 +2] = {x,y+size,z+size};
					stars_positions[loop*6 +3] = {x,y-size,z-size};
					stars_positions[loop*6 +4] = {x,y+size,z+size};
					stars_positions[loop*6 +5] = {x,y-size,z+size};

					stars_uv[loop*6 +0] = {0,0};
					stars_uv[loop*6 +1] = {0,1};
					stars_uv[loop*6 +2] = {1,1};
					stars_uv[loop*6 +3] = {0,0};
					stars_uv[loop*6 +4] = {1,1};
					stars_uv[loop*6 +5] = {1,0};

					intensity = (intfab)*(exit_delay>0||end==true ? (exit_delay/(float)exit_delay_start) : (1.0));
					stars_data[loop*6 +0] = intensity;
					stars_data[loop*6 +1] = intensity;
					stars_data[loop*6 +2] = intensity;
					stars_data[loop*6 +3] = intensity;
					stars_data[loop*6 +4] = intensity;
					stars_data[loop*6 +5] = intensity;
					
					loop++;
				}

				shStars.select().setP(&tProjection).setV(&tView);
				buf_stars_positions.bind().setData(stars_positions.size()*sizeof(Float3), stars_positions.data(), GL_DYNAMIC_DRAW).attrib(0, 3, GL_FLOAT);
				buf_stars_uv.bind().setData(stars_uv.size()*sizeof(Float2), stars_uv.data(), GL_DYNAMIC_DRAW).attrib(1, 2, GL_FLOAT);
				buf_stars_data.bind().setData(stars_data.size()*sizeof(float), stars_data.data(), GL_DYNAMIC_DRAW).attrib(2, 1, GL_FLOAT);
				
				scene->thMan["menu/star"].select();
				buf_stars_positions.draw(GL_TRIANGLES);
			}
			if(exit_delay==0&&end==false)
			{//buttons
				for(auto iter = buttons.begin(); iter!=buttons.end(); iter++){
					iter->second.render(step.getAlpha());
				}
			}
			if(exit_delay==0&&end==false&&step.getTime()>0.5)
			{//version text
				float alpha = step.getTime()-0.5;
				if(alpha<0.0)alpha=0.0;
				if(alpha>1.0)alpha=1.0;
				text.setPosition(scene->a.getAreaWidth()/2, 0).setAlpha(alpha).setBackgroundColor(0.0,0.0,0.0,alpha/3.0).render();
			}
		}
		a->updateAll();
	}
	scene->thMan.removeTexture("menu/background");
	scene->thMan.removeTexture("menu/menubutton");
	scene->thMan.removeTexture("menu/star");
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
		const char * patterns[1] = { "*.gif" };
		const char * name = tinyfd_saveFileDialog("Export animation...", NULL, 1, patterns, NULL);
		if(name!=NULL){
			string str_name = string(name);
			if(str_name.size()>4){
				std::string str_ext = str_name.substr( str_name.length() - 4 );

				ExportInfo info;
				info.name = name;

				createLoading();

				if(str_ext==".gif"){
					ExporterGIF e;
					e.start(this, &info);
				}
				else{
					error("Invalid file extension");
					end=false;
				}

				freeLoading();
				scene->a.makeGLCurrent();
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
				
				if(fileVersion == Menu::fileVersion){//current version
					int frameCount;
					file.read((char*)&frameCount, sizeof(int));

					scene->frameMan.clearFrames();
					//frames
					for(int i=0; i<frameCount/5; i++){
						int width;
						file.read((char*)&width, sizeof(int));//width
						int height;
						file.read((char*)&height, sizeof(int));//height
						int layerCount;
						file.read((char*)&layerCount, sizeof(int));//layer count

						scene->frameMan.selectFrame(scene->frameMan.getFrameCount());
						scene->frameMan.createFrame(width, height);
						for(int j=0; j<layerCount; j++){
							scene->frameMan.getFrame(i)->createLayer();
							file.read((char*)scene->frameMan.getFrame(i)->getLayer(j)->getCanvas()->pixels, width*height*4);//layer pixels
							scene->frameMan.getFrame(i)->getLayer(j)->forceUpdate();
						}
					}
					scene->changeFrame(0);
					scene->frameselector.updateCurrentFrameText();

					exitMenu();
				}
				else{
					error("File version not supported. Supported version: "+to_string(Menu::fileVersion)+". Your version: "+to_string(fileVersion));
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
			
			createLoading();

			//frames
			for(int i=0; i<frameCount; i++){
				Frame * frame = scene->frameMan.getFrame(i);
				int width = frame->getWidth();
				int height = frame->getHeight();

				file.write((char*)&width, sizeof(int));//width
				file.write((char*)&height, sizeof(int));//height

				int layercount = frame->getLayerCount();
				file.write((char*)&layercount, sizeof(int));//layer count

				for(int j=0; j<layercount; j++){
					file.write((char*)frame->getLayer(j)->getCanvas()->pixels, width*height*4);//layer pixels
				}
				updateLoadingStatus(i);
			}

			freeLoading();
			scene->a.makeGLCurrent();

			file.close();
		}
		else{
			error("Cannot create file! Check your permissions or location.");
		}
	}
	dialogEnd();
}
void Menu::actionExit(){
	exit(0);
}

void Menu::updateLoadingStatus(int frame){
	SDL_Event evt;
	while(SDL_PollEvent(&evt)){
		if(evt.type==SDL_QUIT){
			//HALT todo
		}
	}
	SDL_Surface * surf = SDL_GetWindowSurface(loadingWindow);

	SDL_FillRect(surf,NULL,SDL_MapRGB(surf->format, 0, 0, 255));

	SDL_Rect rect;
	rect.x=0;rect.y=0;
	rect.w=(frame/(float)scene->frameMan.getFrameCount())*200;
	rect.h=50;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, 0, 255, 255));

	SDL_UpdateWindowSurface(loadingWindow);
}

void Menu::createLoading(){
	loadingWindow = SDL_CreateWindow("Operation in progress", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 50, 0);
}

void Menu::freeLoading(){
	SDL_DestroyWindow(loadingWindow);
}