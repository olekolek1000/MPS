#include "lib/sdl.h"
#include "app.h"

int main(int argc, char*argv[]){
	App app;
	app.init();

	SDL_Quit();
	return 0;
}
