#include "toolbox.hpp"
#include "lib/opengl.hpp"
#include "shadermanager.hpp"
#include "../editor.hpp"
#include "transform.hpp"
#include "../drawer.hpp"

#include "tool/tool.hpp"
#include "tool/alltools.hpp"

#include "error.hpp"

#include <sstream>
#include <iomanip>

#include "render/func.hpp"

void Toolbox::createTool(Tool * tool){
    tool->initBase(scene);
    tool->init();

    std::stringstream ss;
    ss<<"toolbox/tools/"<<tool->getIconName()<<".png";
	tool->iconLocation = ss.str();

    stringstream header;
    header<<"tools/"<<tool->getIconName();
	tool->iconHeader = header.str();

    tools.push_back(tool);
	
	GuiButton * button = new GuiButton;
	button->init(scene);
	
    buttons.push_back(button);
}

void Toolbox::reloadTextures(){
	if(tools.size()!=buttons.size()){
		error("Toolbox error");
		abort();
	}
	for(uint i=0;i<tools.size();i++){
		scene->thMan.addTexture(tools[i]->iconHeader, tools[i]->iconLocation, TEXSPACE_RGBA, TEXFILTERING_LINEAR);
		buttons[i]->setTexture(&scene->thMan[tools[i]->iconHeader.c_str()]);
	}
}

void Toolbox::init(sceneEditor * scene){
    this->scene = scene;

    createTool(new Pencil);
    createTool(new Line);
    createTool(new Rectangle);
    createTool(new Spray);
    createTool(new FloodFill);
    createTool(new Pipette);
    createTool(new Eraser);
    
	reloadTextures();

    butUndo.init(scene,10,75,48,48).setTexture(&scene->thMan["icons/undo"]);
    butRedo.init(scene,60,75,48,48).setTexture(&scene->thMan["icons/redo"]);
    butMenu.init(scene,25,5,toolboxWidth-50,toolboxWidth-50).setTexture(&scene->thMan["icons/menubutton"]).setBackgroundVisible(false);

    textCurrentTool.init(scene).setAlign(1, 0).setPosition(toolboxWidth/2,130);

    textClock.init(scene);textClock.setPosition(toolboxWidth+10,0);

    setCurrentTool((uint)0);
    previousTool = currentTool;

    int buttonsinrow=2;
    for(uint i=0;i<buttons.size();i++){
        buttons[i]->setSize(48, 48).setPosition(10+(i%buttonsinrow)*50, 150+(i/buttonsinrow)*50);
    }
}

void Toolbox::render(){
    scene->thMan["tb/background1"].select();

    Shader & sh = scene->shGui;
    sh.select();

    {//background
        settingsY = scene->a.getAreaHeight()-scene->frameselector.getHeight()-settingsH;

        xReset(&model);
        xScale(&model,toolboxWidth+8, settingsY);

        sh.setM(&model);
        scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
		scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
		scene->a.square_vert->draw(GL_TRIANGLES);
    }
    {//buttons
        butUndo.render();
        butRedo.render();
        butMenu.render();

        textCurrentTool.render();
        textClock.render();

        for(uint i=0;i<buttons.size();i++){
            buttons[i]->render();
        }
    }

    if(settingsH>0)
    {//settings
        xReset(&model);
        xTranslate(&model, settingsX, settingsY);
        xScale(&model,toolboxWidth+8, settingsH);
        scene->thMan["tb/background1"].select();
        sh.setM(&model);
        scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
		scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
		scene->a.square_vert->draw(GL_TRIANGLES);
    }
    currentTool->render();
}

Toolbox::~Toolbox(){
    for(uint i=0;i<tools.size();i++){
        delete tools[i];
    }
	for(uint i=0;i<buttons.size();i++){
		delete buttons[i];
	}
}

Tool * Toolbox::getCurrentTool(){
    return Toolbox::currentTool;
}
Tool * Toolbox::getPreviousTool(){
    return Toolbox::previousTool;
}

void Toolbox::setCurrentTool(Tool * tool){
    this->currentTool = tool;
    currentTool->select();
    textCurrentTool.changeText(currentTool->getIconName(),scene->a.font16,255,255,255);
}

void Toolbox::setCurrentTool(uint index){
    if(index>=0&&index<tools.size()){
        setSettingsHeight(0);
        currentTool = tools[index];
        currentTool->select();
        scene->drawer.setBrushCircleSize(0);
        textCurrentTool.changeText(currentTool->getIconName(),scene->a.font16,255,255,255);
        scene->actionlog.addMessage(std::string("Tool \""+currentTool->getIconName()+"\" selected.").c_str());
    }
}

void Toolbox::update(){
    currentTool->update();

    if(clockLast+1000<(int)SDL_GetTicks()){
        clockLast=SDL_GetTicks();
        time_t  timev;
        time(&timev);
        struct tm * now = localtime( & timev );
        stringstream ss;
        ss << now->tm_hour;
        ss << ":"<< setfill('0') << setw(2)<<now->tm_min;
        ss << ":"<< setfill('0') << setw(2)<<now->tm_sec;
        textClock.changeText(ss.str(),scene->a.font24,255, 255, 255);
    }
}

int Toolbox::getWidth(){
	return toolboxWidth;
}

bool Toolbox::pushEvent(SDL_Event * evt){
    bool used=false;

    if(evt->type==SDL_MOUSEBUTTONDOWN){
        if(evt->button.x*scene->a.getAreaMultipler()<toolboxWidth&&evt->button.y*scene->a.getAreaMultipler()<toolboxHeight){
            used=true;
        }
    }

    if(currentTool->pushEvent(evt)){
        used=true;
    }

    if(butUndo.pushEvent(evt)){
        if(butUndo.isClicked()){
            scene->drawer.historyUndo();
        }
        used=true;
    }
    if(butRedo.pushEvent(evt)){
        if(butRedo.isClicked()){
            scene->drawer.historyRedo();
        }
        used=true;
    }
    if(butMenu.pushEvent(evt)){
        if(butMenu.isClicked()){
			scene->openMenu();
        }
        used=true;
    }

    for(uint i=0;i<buttons.size();i++){
        if(buttons[i]->pushEvent(evt)){
            used=true;
            if(buttons[i]->isClicked()){
                setCurrentTool(i);
            }
        }
    }

    return used;
}

bool Toolbox::pushGlobalEvent(GlobalEvent evt){
    bool used=false;

    switch(evt){
        case GlobalEvent::GuiChange:{
            setCurrentTool(currentTool);//update text
            used=true;
            break;
        }
        default:break;
    }

    return used;
}

int Toolbox::getSettingsX(){
    return settingsX;
}
int Toolbox::getSettingsY(){
    return settingsY;
}

void Toolbox::setSettingsHeight(int n){
    settingsH=n;
}
