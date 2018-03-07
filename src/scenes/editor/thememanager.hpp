#pragma once
#include "texturemanager.hpp"

#include <string>
#include <map>

struct optVar{
	std::string varstr;
	float varfloat;
};

class sceneEditor;

class ThemeManager{
private:
    std::string themeName;
    std::string defaultThemeName = "default";
    TextureManager texMan;
	std::map<std::string, optVar> vars;
	void loadVariables();
public:
    ThemeManager();
	std::string getVariableS(const char*n);
	float getVariableF(const char*n);
    void addTexture(std::string name, std::string location, int texspace = RGB, int texfiltering = LINEAR);
    void loadTheme(std::string name);
	void reloadThemes(sceneEditor * scene);
    std::string getLocation(std::string location);
    std::string getLocationDefault(std::string location);
    void removeTexture(const char * n);
    Texture & operator [](const char* n);
};
