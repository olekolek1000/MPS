#include "texload.hpp"
#include "defines.hpp"

#include "lib/sdl.hpp"
#include "lib/sdl_image.hpp"
#include "lib/sdl_ttf.hpp"
#include "lib/opengl.hpp"
#include "error.hpp"
#include <sstream>

using namespace std;

SDL_Surface * obraz;
SDL_Surface * obraz_img;

void texRenderText(Texture & tex, TTF_Font * font, const char*text, SDL_Color color, bool blend)
{
	SDL_Surface * surf_text;
	if(blend){
        surf_text = TTF_RenderUTF8_Blended(font,text,color);
	}
	else{
        surf_text = TTF_RenderUTF8_Solid(font,text,color);
	}
	int w,h;
	TTF_SizeUTF8(font,text,&w,&h);
	SDL_Surface * converted = SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32);
	SDL_BlitSurface(surf_text,NULL,converted,NULL);

	tex.select();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,converted->w,converted->h,0,GL_RGBA,GL_UNSIGNED_BYTE,converted->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tex.setWidth(converted->w);
	tex.setHeight(converted->h);
	SDL_FreeSurface(surf_text);
	SDL_FreeSurface(converted);
}

SDL_Surface * surfLoad(std::string lok){
    std::stringstream ss;
    ss<<LOC_ROOT<<lok;
    return IMG_Load(ss.str().c_str());
}