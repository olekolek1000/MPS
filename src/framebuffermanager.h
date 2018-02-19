#pragma once

#include <map>
#include <string>


#define FB_RGB 0
#define FB_RGBA 1

#define FBFILTERING_NEAREST 0
#define FBFILTERING_LINEAR 1

typedef unsigned int GLuint;

class Framebuffer{
private:
	GLuint texture;
	GLuint framebuffer;
	GLuint depthbuffer;
	int width;
	int height;
	bool generated=false;
	bool initialized=false;
	void generate();
	void generateBind();
public:
	void create(int w, int h, int fbspace, int fbfiltering = FBFILTERING_NEAREST);
	void create();
	void free();
	~Framebuffer();
	void select();
	void select(bool clearColor, bool clearDepth, float r, float g, float b, float a);
	GLuint getTexture();
	int getWidth();
	int getHeight();
	void setSize(int w, int h, int fbspace, int fbfiltering = FBFILTERING_NEAREST);
	float getProportions();
};

class FramebufferManager{
private:
	std::map<std::string, Framebuffer>framebuffers;
public:
	void clear();
	Framebuffer & operator [](const char* n);
	~FramebufferManager();
};
