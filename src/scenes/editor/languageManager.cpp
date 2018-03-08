#include "languageManager.hpp"
#include <map>
#include <string.h>
#include "../../error.hpp"
#include <iostream>

bool languageManager::loadFile(std::string filename) {
    if(!configMan.loadFile(filename)) {
        loadRecovery();
        return false;
    } else {
        loadLang();
        return true;
    }
}

void languageManager::loadLang() {
    lang["Lang"] = configMan.getVariable("Lang");
    //Editor
    lang["Pencil"] = configMan.getVariable("Pencil");
    lang["Line"] = configMan.getVariable("Line");
    lang["Spray"] = configMan.getVariable("Spray");
    lang["Floodfill"] = configMan.getVariable("Floodfill");
    lang["Pipette"] = configMan.getVariable("Pipette");
    //Menu
    lang["Go_back"] = configMan.getVariable("Go_back");
    lang["Save_project"] = configMan.getVariable("Save_project");
    lang["Load_project"] = configMan.getVariable("Load_project");
    lang["Export"] = configMan.getVariable("Export");
    lang["Settings"] = configMan.getVariable("Settings");
    lang["Quit"] = configMan.getVariable("Quit");
}

void languageManager::loadRecovery() {
    lang["Lang"] = "Lang";
    //Editor
    lang["Pencil"] = "Pencil";
    lang["Line"] = "Line";
    lang["Spray"] = "Spray";
    lang["Floodfill"] = "Floodfill";
    lang["Pipette"] = "Pipette";
    //Menu
    lang["Go_back"] = "Go_back";
    lang["Save_project"] = "Save_project";
    lang["Load_project"] = "Load_project";
    lang["Export"] = "Export";
    lang["Settings"] = "Settings";
    lang["Quit"] = "Quit";
}