#pragma once

#include <map>
#include <string>
#include "lib/glm.hpp"

typedef unsigned int GLuint;

class Shader{
private:
	GLuint prog;
	GLuint prog_m;
	GLuint prog_v;
	GLuint prog_p;
	GLuint prog_res;
	GLuint prog_prop;
	GLuint prog_time;
	std::map<std::string, GLuint>uniforms;
	bool loaded=false;
public:
	Shader & load(std::string vsh, std::string fsh);
	void free();
	~Shader();
	Shader & setM(glm::mat4*model);
	Shader & setV(glm::mat4*view);
	Shader & setP(glm::mat4*projection);
	Shader & setAttrib(GLuint index, const char * name);
	Shader & setTime(float n);
	Shader & setProportions(float n);
	Shader & setResolution(float w, float h);
	Shader & select();
	Shader & createUniform(const char * name);
	Shader & setUniform(const char * name, glm::mat4 * matrix);
	Shader & setUniform(const char * name, int n);
	Shader & setUniform(const char * name, float n);
	Shader & setUniform(const char * name, float n1, float n2);
	Shader & setUniform(const char * name, float n1, float n2, float n3);
	Shader & setUniform(const char * name, float n1, float n2, float n3, float n4);
	GLuint getUniform(const char * name);
};

class ShaderManager{
private:
	std::map<std::string, Shader>shaders;
public:
	void clear();
	Shader & operator [](const char* n);
	void setP(glm::mat4 * projection);
	void setV(glm::mat4 * view);
	void setTime(float n);
	void setProportions(float n);
	void setResolution(float w, float h);
	~ShaderManager();
};
