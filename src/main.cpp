#include "lib/sdl.hpp"
#include "app.hpp"

int main(int argc, char*argv[]){
	App app;
	app.init();

	SDL_Quit();
	return 0;
}
