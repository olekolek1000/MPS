#include "tool.hpp"
#include "../../drawer.hpp"
#include "../../editor.hpp"

std::string Tool::getIconName(){
    return Tool::iconName;
}

void Tool::initBase(sceneEditor * scene){
    this->scene=scene;
    this->toolbox=&scene->toolbox;
	this->drawer=&scene->drawer;
	this->colorselector=&scene->colorselector;
	this->frameMan=&scene->frameMan;
}
