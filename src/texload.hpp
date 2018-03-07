#ifndef TEXLOAD_H_INCLUDED
#define TEXLOAD_H_INCLUDED

#include "lib/opengl.hpp"
#include "lib/sdl_ttf.hpp"
#include "texturemanager.hpp"
#include <string>

#define RGB 0
#define RGBA 1

#define NEAREST 0
#define LINEAR 1

void texRenderText(Texture & tex, TTF_Font * font, const char*text, SDL_Color color, bool blend=true);

GLuint texLoad(std::string lok, int colorspace, int filtering);

SDL_Surface * surfLoad(std::string lok);

#endif // TEXLOAD_H_INCLUDED
