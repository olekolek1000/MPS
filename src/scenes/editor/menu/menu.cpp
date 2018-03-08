#include "menu.hpp"
#include <vector>

#include "../editor.hpp"
#include "app.hpp"
#include "timestep.hpp"
#include "lib/sdl_image.hpp"
#include "transform.hpp"
#include "render/func.hpp"
#include "menubutton.hpp"

#include "tinyfiledialogs.hpp"

#include "lib/SDL2_rotozoom.hpp"

#include <sstream>
#include <iostream>
#include <map>

#include "error.hpp"

#include "gif.hpp"

#include <fstream>

#include <stdlib.h>

void pix_map::Malloc(size_t size) {
	pixmaps = (int**)malloc(size * sizeof(int*));
	heights = (int*)malloc(size * sizeof(int));
	weights = (int*)malloc(size * sizeof(int));
}

void pix_map::Free() {
	free(pixmaps);
	free(heights);
	free(weights);
}

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
	step.setRate(30);
	std::map<std::string, MenuButton> buttons;
	buttons["01back"].init(this, 200, langMan.lang["Go_back"]);
	buttons["saveproject"].init(this, 300, langMan.lang["Save_project"]);
	buttons["loadproject"].init(this, 400, langMan.lang["Load_project"]);
	buttons["import"].init(this, 500, langMan.lang["Import"]);
	buttons["export"].init(this, 600, langMan.lang["Export"]);
	buttons["99quit"].init(this, 700, langMan.lang["Quit"]);

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
			if(buttons["settings"].isClicked()){
				
			}
			if(buttons["99quit"].isClicked()){
				actionQuit();
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
			{//stars
				xReset(&model);
				xScale(&model, a->getAreaWidth(),a->getAreaHeight()/2);
				shStars.select().setM(&model).setP(&projection).setTime(step.getTime()/8.0).setResolution(a->getAreaWidth()/5.0f,a->getAreaHeight()/5.0f);
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
				a->square_vert->draw(GL_TRIANGLES);
			}
		}

		a->updateAll();
	}
	scene->thMan.removeTexture("menu/background");
	scene->thMan.removeTexture("menu/soon");
	scene->thMan.removeTexture("menu/menubutton");
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
	std::cout << "a";

	dialogPrepare();
	while(1){
		std::cout << "a";

		size_t size = 0;
		const char** patterns = NULL;
		patterns = (const char**)realloc(patterns,size++);

		patterns[size-1] = "*.*";

		for(auto& i : io_library_manager) {
			const char* format = i.first.c_str();
			
			patterns = (const char**)realloc(patterns,(size++) * sizeof(const char*));
			patterns[size-1] = format;
		}
		const char * name = tinyfd_saveFileDialog("Export animation...", NULL, size, patterns, NULL);
		free(patterns);

		if(name!=NULL) {
			std::string str = std::string(name);
			if(str.size() > 4) {
				str = str.substr(str.length() - 3);
				loaded:
				if(io_library_manager[str].isLoaded()) {
					Module::export_ft exp_func = (Module::export_ft)io_library_manager[str].get_function("export");

					current_export_data.Malloc(scene->frameMan.getFrameCount());

					for(int i = 0; i < scene->frameMan.getFrameCount(); i++) {
						SDL_Surface* pixmap = scene->frameMan.getFrame(i)->getOverlay();

						current_export_data.pixmaps[i] = (int*)pixmap->pixels;
						current_export_data.heights[i] = pixmap->h;
						current_export_data.weights[i] = pixmap->w;

					}

					exp_func((const int**)current_export_data.pixmaps,current_export_data.heights,current_export_data.weights,scene->frameMan.getFrameCount(),&current_export_thread_progress);

					current_export_data.Free();

				}
				else {
					std::string es = std::string("Module responsible for: ") + str + std::string(" isn't loaded.\nYou can choose it manually after this message.");
					error(es.c_str());

					if(LoadModule()) goto loaded;
					else error("Cannot load module");
				}
			}
		}
		else {
			break;
		}
	}
	dialogEnd();
}

bool Menu::LoadModule() {
	const char* format = "*.mpsm";
	const char* path = tinyfd_openFileDialog("Select module...", NULL, 1, &format, "Moving Picture Studio Module (*.mpsm)", false);
	if(path != NULL) {
		std::string str(path);
		if(str.size() > 4) {
			std::string sec_str = str.substr(str.length() - 3);
			Module mod;
			if(mod.load(path)) {
				if(mod.get_function("info") && mod.get_function("export")) {
					
					Module::info_ft inf_func = (Module::info_ft)mod.get_function("info");
					const char* formats = inf_func();

					io_library_manager[std::string(formats)] = mod;
					return true;
				}

			}
		}
	}
	return false;
}

void Menu::actionImport(){
	dialogPrepare();

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