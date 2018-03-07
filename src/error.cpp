#include "error.hpp"
#include "lib/sdl.hpp"

void error(std::string text){
	SDL_ShowSimpleMessageBox(0,"Error",text.c_str(),0);
} 
