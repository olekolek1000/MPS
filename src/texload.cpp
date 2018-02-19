#include "texload.h"
#include "defines.h"

#include "lib/sdl.h"
#include "lib/sdl_image.h"
#include "lib/sdl_ttf.h"
#include "lib/opengl.h"
#include "error.h"
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

GLuint texLoad(std::string lok, int colorspace, int filtering)
{
    std::stringstream ss;
    ss<<LOC_ROOT<<lok;

    obraz=NULL;
    obraz_img=NULL;

    obraz_img = IMG_Load(ss.str().c_str());

    if(obraz_img==NULL)
    {
        std::stringstream ss2;
        ss2<<"TextureLoader: Not found/error: \""<<ss.str()<<"\"";
        error(ss2.str());
        return 0;
    }

    GLuint tex=0;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    if(colorspace==RGB)
    {
        obraz = SDL_ConvertSurfaceFormat(obraz_img,SDL_PIXELFORMAT_RGB24,0);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,obraz->w,obraz->h,0,GL_RGB,GL_UNSIGNED_BYTE,obraz->pixels);
    }
    else if(colorspace==RGBA)
    {
        obraz = SDL_ConvertSurfaceFormat(obraz_img,SDL_PIXELFORMAT_RGBA32,0);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,obraz->w,obraz->h,0,GL_RGBA,GL_UNSIGNED_BYTE,obraz->pixels);
    }
    else
    {
        error("Invalid texture colorspace");
    }

    if(filtering==NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else if(filtering==LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        error("Invalid filtering value");
    }

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    SDL_FreeSurface(obraz_img);
    SDL_FreeSurface(obraz);

    return tex;
}
