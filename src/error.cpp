#include "error.h"
#include "lib/sdl.h"

void error(std::string text){
	SDL_ShowSimpleMessageBox(0,"Error",text.c_str(),0);
} 
