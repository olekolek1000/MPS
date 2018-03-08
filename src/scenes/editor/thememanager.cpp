#include "thememanager.hpp"

#include <fstream>
#include <sstream>
#include "../../error.hpp"
#include "../../defines.hpp"
#include "configmanager.hpp"

#include <string>
#include <iostream>
#include <iomanip>

typedef unsigned int uint;

ThemeManager::ThemeManager(){
	themeName = defaultThemeName;
	loadVariables();
    //std::fstream file;
    configMan.loadFile(std::string(LOC_ROOT)+std::string("/editor/themes/default.txt"));
    //file.open(std::string(std::string(LOC_ROOT)+std::string("/editor/themes/default.txt")).c_str(), std::ios::in);
    //std::string theme = configMan.getVariable("Theme");
    std::string theme = configMan.getVariable("Theme");
    //std::getline(file, theme);
    //file.close();
    loadTheme(theme);
}

std::string ThemeManager::getVariableS(const char*n){
	return vars[n].varstr;
}
float ThemeManager::getVariableF(const char*n){
	return vars[n].varfloat;
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
	std::fstream file;
	file.open(loc.str().c_str(), std::ios::in);
	if(file.good()){
		while(!file.eof()){
			std::string line;
			std::getline(file, line);
			std::stringstream name;
			uint i=0;
			while(i<line.size()&&line[i]!='='){
				name<<line[i];
				i++;
			}
			std::stringstream variable;
			i++;
			while(i<line.size()){
				variable<<line[i];
				i++;
			}
            
    		vars[name.str()].varstr = variable.str();
			vars[name.str()].varfloat = std::atof(variable.str().c_str());
		}
	}
	file.close();
}

void ThemeManager::loadTheme(std::string name){
    ThemeManager::themeName = name;
	
	loadVariables();

    addTexture("background","background.png",RGB);
    addTexture("button","button.png",RGBA);
    addTexture("cs/point","colorselector/point.png",RGBA,NEAREST);
    addTexture("cs/scroll","colorselector/scroll.png",RGBA,NEAREST);
    addTexture("fs/background1","frameselector/background1.png",RGBA);
    addTexture("fs/background_top","frameselector/background_top.png",RGBA);
    addTexture("fs/opt_blank","frameselector/opt_blank.png",RGBA, NEAREST);
    addTexture("fs/opt_duplicate","frameselector/opt_duplicate.png",RGBA, NEAREST);
    addTexture("fs/newframe","frameselector/newframe.png",RGBA, NEAREST);
    addTexture("fs/removeframe","frameselector/removeframe.png",RGBA, NEAREST);
    addTexture("icons/down","icons/down.png",RGBA);
    addTexture("icons/left","icons/left.png",RGBA);
    addTexture("icons/up","icons/up.png",RGBA);
    addTexture("icons/right","icons/right.png",RGBA);
    addTexture("icons/undo","icons/undo.png", RGBA);
    addTexture("icons/redo","icons/redo.png", RGBA);
    addTexture("icons/menubutton","icons/menubutton.png",RGBA);
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


std::string ThemeManager::getLocationDefault(std::string location){
    std::stringstream ss;
    ss<<"editor/themes/"<<defaultThemeName<<"/"<<location;
    return ss.str();
}

Texture& ThemeManager::operator [](const char* n){
	return texMan[n];
}

/* THEME RELOADING */
#include "editor.hpp"
void ThemeManager::reloadThemes(sceneEditor * scene){
	scene->toolbox.reloadTextures();
	scene->drawer.reloadTextures();
}