#pragma once

#ifdef __linux__
#define GLEW_STATIC
#include <GL/glew.h>
//#include <SDL2/SDL_opengles2.h>
#endif

/*
#ifdef __ANDROID__
#include <SDL2/SDL_opengles2.h>
#endif*/ 
 
#ifdef __MINGW32__
//#include <SDL2/SDL_opengles2.h>

#define GLEW_STATIC
#include <GL/glew.h>
#endif
