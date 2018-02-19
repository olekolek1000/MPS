#include "texturemanager.h"
#include "lib/sdl.h"
#include "lib/sdl_image.h"
#include "lib/opengl.h"
#include "error.h"
#include "defines.h"

#include "lib/SDL2_rotozoom.h"//tymczasowo chyba, trzeba odwracac tekstury

void Texture::create(){
    if(loaded){
        free();
    }
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    loaded=true;
}

void Texture::create(int texfiltering, int texwrap){
    if(loaded){
        free();
    }
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    switch(texfiltering){
        case TEXFILTERING_NEAREST:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }
        case TEXFILTERING_LINEAR:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        case TEXFILTERING_NEAREST_LINEAR:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        case TEXFILTERING_LINEAR_NEAREST:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }
        default:{
            error("Texture filtering not found");
            exit(1);
            break;
        }
    }
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texwrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texwrap);

    loaded=true;
}

GLuint Texture::getTexture(){
    return texture;
}

void Texture::setWidth(int n){
    width=n;
}

void Texture::setHeight(int n){
	height=n;
}

void Texture::load(SDL_Surface * surf, int texspace, int texfiltering){
    if(loaded){
        free();
    }
    width=surf->w;
    height=surf->h;
    SDL_Surface * converted;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    switch(texspace){
        case TEXSPACE_RGB:{
            converted = SDL_ConvertSurfaceFormat(surf,SDL_PIXELFORMAT_RGB24,0);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,converted->w,converted->h,0,GL_RGB,GL_UNSIGNED_BYTE,converted->pixels);
            break;
        }
        case TEXSPACE_RGBA:{
            converted = SDL_ConvertSurfaceFormat(surf,SDL_PIXELFORMAT_RGBA32,0);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,converted->w,converted->h,0,GL_RGBA,GL_UNSIGNED_BYTE,converted->pixels);
            break;
        }
        default:{
            error("Texture space not found");
            exit(1);
            break;
        }
    }
    SDL_FreeSurface(converted);
    switch(texfiltering){
        case TEXFILTERING_NEAREST:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }
        case TEXFILTERING_LINEAR:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        case TEXFILTERING_NEAREST_LINEAR:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        case TEXFILTERING_LINEAR_NEAREST:{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }
        default:{
            error("Texture filtering not found");
            exit(1);
            break;
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    loaded=true;
}

void Texture::load(const char * location, int texspace, int texfiltering){
	if(loaded){
		free();
	}
	SDL_Surface * raw = IMG_Load(std::string(std::string(LOC_ROOT)+std::string(location)).c_str());
	if(raw==NULL){
		error(std::string(std::string("Cannot load image: ")+std::string(location)));
	}
	else{
		load(raw, texspace, texfiltering);
		SDL_FreeSurface(raw);
	}
}

void Texture::update(SDL_Surface * surf){
    select();
    if(surf->format->BitsPerPixel==32){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surf->w,surf->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surf->pixels);
    }
    else if(surf->format->BitsPerPixel==24){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,surf->w,surf->h,0,GL_RGB,GL_UNSIGNED_BYTE,surf->pixels);
    }
    else{
        error("Unknown texture space.");
    }
}
void Texture::update(unsigned char * data, int width, int height, int texspace){
	select();
    if(texspace==TEXSPACE_RGBA){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    }
    else if(texspace==TEXSPACE_RGB){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    }
    else{
        error("Unknown texture space.");
    }
}

void Texture::free(){
	if(loaded==true){
        Texture::select();
		glDeleteTextures(1,&texture);
		loaded=false;
	}
}

Texture::~Texture(){
	free();
}

void Texture::select(){
	glBindTexture(GL_TEXTURE_2D, texture);
}

int Texture::getWidth(){
	return width;
}

int Texture::getHeight(){
	return height;
}

float Texture::getProportions(){
	return (float)width/(float)height;
}

float Texture::getProportionsRev(){
	return (float)height/(float)width;
}

void TextureManager::clear(){
	textures.clear();
}

void TextureManager::remove(const char * n){
    textures.erase(n);
}

Texture & TextureManager::operator [](const char* n){
	return textures[n];
}

TextureManager::~TextureManager(){
	TextureManager::clear();
}
