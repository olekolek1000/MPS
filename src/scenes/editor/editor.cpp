#include "editor.hpp"
#include "lib/opengl.hpp"
#include "transform.hpp"
#include "texload.hpp"
#include "toolbox/toolbox.hpp"

#include "render/func.hpp"

#include "lib/sdl_image.hpp"
#include "error.hpp"
#include "menu/menu.hpp"

#include <iostream>

void sceneEditor::setProjection(){
    int w = a.getAreaWidth();
    int h = a.getAreaHeight();

	projection = glm::ortho(0.0f,a.getProportions(),1.0f,0.0f);
	shMan.setProportions(a.getProportions());
	shMan.setResolution(w, h);

	guiProjection = glm::ortho(0.0f, (float)a.getAreaWidth(), (float)a.getAreaHeight(), 0.0f);
	shGui.select();shGui.setP(&guiProjection);
	shGuiColor.select();shGuiColor.setP(&guiProjection);
	
	drawer.updateViewport();
}
  
void sceneEditor::setCursor(std::string name){
    SDL_SetCursor(cursors[name]);
}  
 
sceneEditor::~sceneEditor(){
    for(auto iter = cursors.begin(); iter!=cursors.end(); iter++){
        SDL_FreeCursor(iter->second);
    }
    cursors.clear();
} 

void sceneEditor::load(){
    cursors["arrow"] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    cursors["crosshair"] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    cursors["hand"] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Surface * surf = surfLoad("editor/dragcursor.png");
    cursors["drag"] = SDL_CreateColorCursor(surf, 12, 12);
    SDL_FreeSurface(surf);

    shGui.load("editor/gui.vsh","editor/gui.fsh");

    shGuiColor.load("editor/guicolor.vsh","editor/guicolor.fsh");
    shGuiColor.createUniform("COLOR");
	
	shMan["overlay"].load("editor/overlay.vsh","editor/overlay.fsh").createUniform("OVERLAY_RES").createUniform("ALPHA");
    shMan["overlaybg"].load("editor/overlaybg.vsh","editor/overlaybg.fsh").createUniform("OVERLAY_RES").createUniform("ZOOM");
    shMan["overlaygrid"].load("editor/overlaygrid.vsh","editor/overlaygrid.fsh").createUniform("OVERLAY_RES").createUniform("ZOOM");
	shMan["overlayborder"].load("editor/overlayborder.vsh","editor/overlayborder.fsh").setAttrib(0, "inPosition").setAttrib(1, "inTexcoord");

    frameMan.createFrame(320, 240);
    frameMan.selectFrame(0);

    drawer.init(this);drawer.setCurrentFrame(frameMan.getCurrentFrame());
    toolbox.init(this);
    colorselector.init(this);
    frameselector.init(this);

	if(mpsConfig.loadFile("mps.conf") == 0) {
		langMan.loadRecovery();
		error("Can't load config!");
	} else {
		if(langMan.loadFile(std::string("../assets/")+std::string("/languages/")+std::string(mpsConfig.getVariable("Lang"))+std::string(".lang")) == 0) {
			error("Can't load language!");
		}
	}

    step.setRate(30);
    setProjection();
}

void sceneEditor::openMenu(){
	menuopenrequest=true;
}

void sceneEditor::loop(){
    SDL_Event evt;
    bool end=false;
	
    while(!end){
        while(a.eventHandle(&evt)){
			switch(evt.type){
			    case SDL_KEYDOWN:{
                    switch(evt.key.keysym.sym){
                        case SDLK_LEFT:{
                            changeFrame(frameMan.getCurrentFrameIndex()-1);
                            break;
                        }
                        case SDLK_RIGHT:{
                            changeFrame(frameMan.getCurrentFrameIndex()+1);
                            break;
                        }
						case SDLK_RETURN:{
							thMan.loadTheme("default");
							thMan.reloadThemes(this);
							break;
						}
						case SDLK_END:{
							end=true;
							break;
						}
						case SDLK_ESCAPE:{
							openMenu();
							break;
						}
                    }
                    break;
			    }
			}

			if(!toolbox.pushEvent(&evt)){
				if(!colorselector.pushEvent(&evt)){
					if(!frameselector.pushEvent(&evt)){
						drawer.pushEvent(&evt);
					}
				}
			}
		}
		while(a.onEvent()){
			switch(a.getEvent()){
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
            drawer.update();
		    toolbox.update();
            frameselector.update();
		}

		if(!a.isHidden())
		{//render
			a.shMan.setP(&projection);
			shMan.setP(&projection);
			shMan.setTime(step.getTime());

			Shader & shader = a.shMan["default2d"];

            {//background
                thMan["background"].select();
                xReset(&model);
                if(a.getProportions()>1.0){
                    xScale(&model, a.getProportions(),a.getProportions());
                }
                else{
                    xScale(&model, 1.0,1.0);
                }
                shader.select().setM(&model);
				a.square_vert->bind().attrib(0,2,GL_FLOAT);
				a.square_uv->bind().attrib(1,2,GL_FLOAT);
				a.square_vert->draw(GL_TRIANGLES);
            }

            drawer.render(step.getAlpha());
            toolbox.render();
            colorselector.render();
            frameselector.render(step.getAlpha());
		}
		
		if(menuopenrequest){
			menuopenrequest=false;
			Menu menu(this);
			menu.loop();
			step.reset();
			setProjection();
		}
		else{
			a.updateAll();
		}
    }
}

void sceneEditor::changeFrame(int n){
    frameMan.selectFrame(n);
    drawer.setCurrentFrame(frameMan.getCurrentFrame());
    drawer.updateViewport();
}
