#include <map>
#include <string.h>
#include "configmanager.hpp"

class languageManager {
public:
    std::map<std::string, std::string> lang;
    bool loadFile(std::string filename);
    void loadRecovery();
private:
    configManager configMan;
    void loadLang();
};