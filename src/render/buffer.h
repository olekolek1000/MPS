#pragma once

#include "lib/opengl.h"

class RBuffer{
	GLuint buffer;
	unsigned int size;
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