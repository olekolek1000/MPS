#pragma once

#include "lib/opengl.h"

class RBuffer{
	GLuint buffer;
	unsigned int size=0;
	unsigned int drawsize=0;
public:
	GLuint getBuffer();
	GLuint getSize();
	RBuffer& bind();
	RBuffer& setData(GLsizeiptr size, const GLvoid * data, GLenum usage);
	RBuffer& attrib(GLuint index, GLuint size, GLuint type);
	RBuffer& draw(GLenum mode);
	RBuffer();
	~RBuffer();
};