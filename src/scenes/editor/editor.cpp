#include "editor.hpp"
#include "lib/opengl.hpp"
#include "transform.hpp"
#include "texload.hpp"
#include "toolbox/toolbox.hpp"

#include "render/func.hpp"

#include "lib/sdl_image.hpp"
#include "error.hpp"
#include "menu/menu.hpp"

#include <sstream>
#include <time.h>

#ifdef __linux__
void setEditorPresence(DiscordRPC* discord_status) {
	DiscordRichPresence editorPresence;
	memset(&editorPresence, 0, sizeof(editorPresence));
	editorPresence.state = "Edits something";
    editorPresence.largeImageKey = "drawing";
	editorPresence.largeImageText = "In MPS editor";
	editorPresence.smallImageKey = "icon";
	editorPresence.smallImageText = "Moving Picture Studio";
    editorPresence.instance = 1;
    editorPresence.startTimestamp = time(0);
	discord_status->updateStatus(&editorPresence);
}
#endif

#include "gui/checkbox.hpp"

void sceneEditor::setProjection(){
    int w = a.getAreaWidth();
    int h = a.getAreaHeight();

	projection = glm::ortho(0.0f,a.getProportions(),1.0f,0.0f);
	shMan.setProportions(a.getProportions());
	shMan.setResolution(w, h);

	guiProjection = glm::ortho(0.0f, (float)a.getAreaWidth(), (float)a.getAreaHeight(), 0.0f);
	shGui.select().setP(&guiProjection);
	shGuiColor.select().setP(&guiProjection);
	shGuiAlpha.select().setP(&guiProjection);

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

SDL_Cursor * newcursor(std::string location, int x, int y){
	SDL_Surface * surf = surfLoad(location);
	SDL_Cursor * cur = SDL_CreateColorCursor(surf, x, y);
	SDL_FreeSurface(surf);
	return cur;
}

void sceneEditor::load(){
    cursors["arrow"] = newcursor("editor/arrowcursor.png", a.config.getvarI("cursor_arrow_centerX"),a.config.getvarI("cursor_arrow_centerY"));
    cursors["crosshair"] = newcursor("editor/crosshaircursor.png", a.config.getvarI("cursor_crosshair_centerX"),a.config.getvarI("cursor_crosshair_centerY"));
    cursors["drag"] = newcursor("editor/dragcursor.png", a.config.getvarI("cursor_drag_centerX"),a.config.getvarI("cursor_drag_centerY"));

    shGui.load("editor/gui.vsh","editor/gui.fsh");
	shGuiAlpha.load("editor/guialpha.vsh","editor/guialpha.fsh").createUniform("ALPHA");
    shGuiColor.load("editor/guicolor.vsh","editor/guicolor.fsh").createUniform("COLOR");

	shMan["overlay"].load("editor/overlay.vsh","editor/overlay.fsh").createUniform("OVERLAY_RES").createUniform("ALPHA");
    shMan["overlaybg"].load("editor/overlaybg.vsh","editor/overlaybg.fsh").createUniform("OVERLAY_RES").createUniform("ZOOM");
    shMan["overlaygrid"].load("editor/overlaygrid.vsh","editor/overlaygrid.fsh").createUniform("OVERLAY_RES").createUniform("ZOOM");
	shMan["overlayborder"].load("editor/overlayborder.vsh","editor/overlayborder.fsh");
	shMan["brushcircle"].load("editor/brushcircle.vsh","editor/brushcircle.fsh");

    frameMan.createFrame(a.config.getvarI("newframe_width"), a.config.getvarI("newframe_height"))->createLayer();
    frameMan.selectFrame(0);

	actionlog.init(this);
    drawer.init(this);drawer.setCurrentFrame(frameMan.getCurrentFrame());drawer.setCameraPosition(-a.config.getvarI("newframe_width")/2.0, -a.config.getvarI("newframe_height")/2.0);drawer.setZoomPixelPerfect(1);
    toolbox.init(this);
    colorselector.init(this);
    frameselector.init(this);
	layerMan.init(this);layerMan.setCurrentFrame(frameMan.getCurrentFrame());

	if(a.config.getvarI("rpc") == 1) {
		discord_status.activeRPC = true;
		discord_status.init();
		#ifdef __linux__
		discord_status.init();
		setEditorPresence(&discord_status);
		#endif
	} else {
		discord_status.activeRPC = false;
	}

	actionlog.addMessage("Program started successfully.", 3.0f);
	actionlog.addMessage("Press ESC to open menu.", 3.0f);

    step.setRate(30);
	step.setSpeed(a.config.getvarF("animationSpeed"));
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
							actionlog.addMessage("Theme reloaded and changed to Default");
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
						case SDLK_EQUALS:{
							if(a.getAreaMultipler()>0.1){
								a.setAreaMultipler(a.getAreaMultipler()-0.05);
							}
							break;
						}
						case SDLK_MINUS:{
							if(a.getAreaMultipler()<2.0){
								a.setAreaMultipler(a.getAreaMultipler()+0.05);
							}
							break;
						}
                    }
                    break;
			    }
			}

			if(evt.type == SDL_FINGERDOWN) {
				char prsr[sizeof(float)];
				size_t size = sizeof(float);
				snprintf(prsr, size, "%f", evt.tfinger.pressure);
				printf("Prsr: %f", evt.tfinger.pressure);
			}

			if(!toolbox.pushEvent(&evt)){
				if(!colorselector.pushEvent(&evt)){
					if(!frameselector.pushEvent(&evt)){
						if(!layerMan.pushEvent(&evt)){
							drawer.pushEvent(&evt);
						}
					}
				}
			}
		}
		while(a.onEvent()){
			GlobalEvent evt = a.getEvent();
			switch(evt){
				case GlobalEvent::WindowSizeChange:{
					setProjection();
					break;
				}
				case GlobalEvent::Exit:{
				    exit(0);
					end=true;
					break;
				}
				case GlobalEvent::DebuggerToggle:{
					std::stringstream ss;ss<<"Debugger: ";a.isDebuggerOpened() ? ss<<"YES" : ss<<"NO";
					actionlog.addMessage(ss.str().c_str());
					break;
				}
				case GlobalEvent::FullscreenToggle:{
					std::stringstream ss;ss<<"Fullscreen: ";a.isFullscreen() ? ss<<"YES" : ss<<"NO";
					actionlog.addMessage(ss.str().c_str());
					break;
				}
				case GlobalEvent::VSyncToggle:{
					std::stringstream ss;ss<<"VSync: ";a.isVSyncActive() ? ss<<"YES" : ss<<"NO";
					actionlog.addMessage(ss.str().c_str());
					break;
				}
				case GlobalEvent::FPSLimitToggle:{//go to next
				}
				case GlobalEvent::FPSLimit:{
					std::stringstream ss;ss<<"FPS limit: ";a.isFPSLimitActive() ? ss<<"YES" : ss<<"NO";
					ss<<", limit: "<<a.getFPSLimit();
					actionlog.addMessage(ss.str().c_str());
					break;
				}
				case GlobalEvent::GuiChange:{
					std::stringstream ss;ss<<"GUI size: "<<(1.0/a.getAreaMultipler()*100)<<"%";
					actionlog.addMessage(ss.str().c_str());
					break;
				}
				default:{
					break;
				}
			} 
			toolbox.pushGlobalEvent(evt);
			frameselector.pushGlobalEvent(evt);
			colorselector.pushGlobalEvent(evt);
			actionlog.pushGlobalEvent(evt);
		}
		while(step.onUpdate())
		{//update
            drawer.update();
		    toolbox.update();
            frameselector.update();
			layerMan.update();
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
			layerMan.render();
			actionlog.render();
		}
		if(menuopenrequest){
			menuopenrequest=false;
			Menu menu(this);
			menu.loop();
			#ifdef __linux__
			setEditorPresence(&discord_status);
			#endif
			step.reset();
			setProjection();
		}
		else{
			a.updateAll();
		}
    }
	#ifdef __linux__
	discord_status.exit();
	#endif
}

void sceneEditor::changeFrame(int n){
    frameMan.selectFrame(n);
	layerMan.setCurrentFrame(frameMan.getCurrentFrame());
    drawer.setCurrentFrame(frameMan.getCurrentFrame());
    drawer.updateViewport();
}
