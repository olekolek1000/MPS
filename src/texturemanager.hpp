#pragma once

#include <string>
#include <map>
#include "lib/sdl.hpp"
#include "lib/opengl.hpp"

#define TEXSPACE_RGB 0
#define RGB 0
#define TEXSPACE_RGBA 1
#define RGBA 1

#define TEXFILTERING_NEAREST 0
#define NEAREST 0
#define TEXFILTERING_LINEAR 1
#define LINEAR 1

#define TEXFILTERING_NEAREST_LINEAR 2
#define TEXFILTERING_LINEAR_NEAREST 3

typedef unsigned int GLuint;

class Texture{
private:
	GLuint texture;
	int width;
	int height;
public:
	void load(const char * location, int texspace = TEXSPACE_RGB, int texfiltering = TEXFILTERING_LINEAR);
	void load(SDL_Surface * surf, int texspace = TEXSPACE_RGB, int texfiltering = TEXFILTERING_LINEAR);
	void create();
	void create(int texfiltering, int texwrap = GL_CLAMP_TO_EDGE);
	void free();
	~Texture();
	void select();//'aka "bind"
	GLuint getTexture();
	int getWidth();
	int getHeight();
	void update(SDL_Surface * surf);
	void update(unsigned char * data, int width, int height, int texspace);
	void setWidth(int n);
	void setHeight(int n);
	float getProportions();
	float getProportionsRev();
	bool loaded=false;
};

class TextureManager{
private:
	std::map<std::string, Texture>textures;
public:
	void clear();
	void remove(const char * n);
	Texture & operator [](const char* n);
	~TextureManager();
};
