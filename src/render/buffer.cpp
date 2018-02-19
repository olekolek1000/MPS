#include "buffer.h"
#include "lib/opengl.h"

RBuffer::RBuffer(){
	glGenBuffers(1, &buffer);
}

RBuffer::~RBuffer(){
	glDeleteBuffers(1,&buffer);
}

GLuint RBuffer::getBuffer(){
	return buffer;
}

GLuint RBuffer::getSize(){
	return size;
}

RBuffer& RBuffer::bind(){
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	return *this;
}

RBuffer& RBuffer::attrib(GLuint index, GLuint size, GLuint type){
	glVertexAttribPointer(index,size,type,GL_FALSE,0,(void*)0);
	return *this;
}

RBuffer& RBuffer::setData(GLsizeiptr size, const GLvoid * data, GLenum usage){
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	this->size = size;
	return *this;
}

RBuffer& RBuffer::draw(GLenum mode){
	glDrawArrays(mode, 0, size);
	return *this;
}