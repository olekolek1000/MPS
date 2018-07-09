#include "thememanager.hpp"

#include <fstream>
#include <sstream>
#include "error.hpp"
#include "defines.hpp"
#include "editor.hpp"

typedef unsigned int uint;

void ThemeManager::init(sceneEditor * scene){
    this->scene = scene;
    loadTheme(scene->a.config.getvarS("theme"));
}

void ThemeManager::addTexture(std::string name, std::string location, int texspace, int texfiltering){
    std::stringstream loc;
    loc<<LOC_ROOT<<"/editor/themes/"<<themeName<<"/"<<location;
    std::fstream file;
    file.open(loc.str().c_str(), std::ios::in);
    if(file.good()){
        texMan[name.c_str()].load(getLocation(location).c_str(),texspace, texfiltering);
    }
    else{
        texMan[name.c_str()].load(getLocationDefault(location).c_str(),texspace, texfiltering);
    }
    file.close();

}

void ThemeManager::loadVariables(){
	std::stringstream loc;
    loc<<LOC_ROOT<<"/editor/themes/"<<themeName<<"/theme.cfg";
    config.open(loc.str().c_str());
    if(!config.isGood()){
        error("Cannot find configuration file in theme named "+themeName);
        ThemeManager::themeName=defaultThemeName;
        loc.str("");
        loc<<LOC_ROOT<<"/editor/themes/"<<defaultThemeName<<"/theme.cfg";
        config.open(loc.str().c_str());
        if(!config.isGood()){
            error("Cannot find default theme configuration file!");
            exit(0);
        }
    }
}

void ThemeManager::loadTheme(std::string name){
    ThemeManager::themeName = name;
	
	loadVariables();

    addTexture("background","background.png",RGB);
    addTexture("button","button.png",RGBA);
    addTexture("checkbox","checkbox.png",RGBA);
    addTexture("cs/point","colorselector/point.png",RGBA);
    addTexture("cs/scroll","colorselector/scroll.png",RGBA,NEAREST);
    addTexture("fs/background1","frameselector/background1.png",RGBA);
    addTexture("fs/background_top","frameselector/background_top.png",RGBA);
    addTexture("fs/opt_blank","frameselector/opt_blank.png",RGBA);
    addTexture("fs/opt_duplicate","frameselector/opt_duplicate.png",RGBA);
    addTexture("fs/newframe","frameselector/newframe.png",RGBA);
    addTexture("fs/removeframe","frameselector/removeframe.png",RGBA);

    addTexture("ls/select","layerselector/select.png",RGBA);
    addTexture("ls/visible","layerselector/visible.png", RGBA);
    addTexture("ls/invisible","layerselector/invisible.png",RGBA);

    addTexture("icons/down","icons/down.png",RGBA);
    addTexture("icons/left","icons/left.png",RGBA);
    addTexture("icons/up","icons/up.png",RGBA);
    addTexture("icons/right","icons/right.png",RGBA);
    addTexture("icons/undo","icons/undo.png", RGBA);
    addTexture("icons/redo","icons/redo.png", RGBA);
    addTexture("icons/menubutton","icons/menubutton.png",RGBA);
    addTexture("icons/checkbox","icons/checkbox.png",RGBA);
    addTexture("tb/background1","toolbox/background1.png",RGBA);
}

void ThemeManager::removeTexture(const char * n){
    texMan.remove(n);
}

std::string ThemeManager::getLocation(std::string location){
    std::stringstream ss;
    ss<<"editor/themes/"<<themeName<<"/"<<location;
    return ss.str();
}

std::string ThemeManager::getThemeName(){
    return themeName;
}

std::string ThemeManager::getLocationDefault(std::string location){
    std::stringstream ss;
    ss<<"editor/themes/"<<defaultThemeName<<"/"<<location;
    return ss.str();
}

Texture& ThemeManager::operator [](const char* n){
	return texMan[n];
}

/* THEME RELOADING */
void ThemeManager::reloadThemes(){
	scene->toolbox.reloadTextures();
	scene->drawer.reloadTextures();
}