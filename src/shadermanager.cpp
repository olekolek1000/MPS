#include "shadermanager.hpp"
#include "shaderload.hpp"

Shader & Shader::load(std::string vsh, std::string fsh){
	if(loaded==false){
		prog = shaderLoad(vsh.c_str(),fsh.c_str());
		prog_m = glGetUniformLocation(prog, "M");
		prog_v = glGetUniformLocation(prog, "V");
		prog_p = glGetUniformLocation(prog, "P");
		prog_res = glGetUniformLocation(prog, "RESOLUTION");
		prog_prop = glGetUniformLocation(prog, "PROPORTIONS");
		prog_time = glGetUniformLocation(prog, "TIME");
		Shader::select();
		loaded=true;
	}
	return *this;
}

Shader & Shader::setAttrib(GLuint index, const char * name){
	glBindAttribLocation(prog, index, name);
	return *this;
}

void Shader::free(){
	if(loaded==true){
	    Shader::select();
		glDeleteProgram(prog);
		loaded=false;
	}
}

Shader::~Shader(){
	free();
}

Shader & Shader::select(){
	glUseProgram(prog);
	return *this;
}

Shader & Shader::setM(glm::mat4* model){
	glUniformMatrix4fv(prog_m, 1, GL_FALSE,(const GLfloat*)&model[0][0]);
	return *this;
}

Shader & Shader::setV(glm::mat4* view){
	glUniformMatrix4fv(prog_v, 1, GL_FALSE,(const GLfloat*)&view[0][0]);
	return *this;
}

Shader & Shader::setP(glm::mat4* projection){
	glUniformMatrix4fv(prog_p, 1, GL_FALSE,(const GLfloat*)&projection[0][0]);
	return *this;
}

Shader & Shader::setProportions(float n){
	glUniform1f(prog_prop,n);
	return *this;
}

Shader & Shader::setResolution(float w, float h){
	glUniform2f(prog_res,w,h);
	return *this;
}

Shader & Shader::setTime(float n){
	glUniform1f(prog_time, n);
	return *this;
}

Shader & Shader::createUniform(const char * name){
	uniforms[name] = glGetUniformLocation(prog, name);
	return *this;
}

Shader & Shader::setUniform(const char * name, glm::mat4 * matrix){
	glUniformMatrix4fv(uniforms[name], 1, GL_FALSE,(const GLfloat*)&matrix[0][0]);
	return *this;
}

Shader & Shader::setUniform(const char * name, float n){
	glUniform1f(uniforms[name],n);
	return *this;
}

Shader & Shader::setUniform(const char * name, int n){
	glUniform1i(uniforms[name],n);
	return *this;
}

Shader & Shader::setUniform(const char * name, float n1, float n2){
	glUniform2f(uniforms[name],n1, n2);
	return *this;
}

Shader & Shader::setUniform(const char * name, float n1, float n2, float n3){
	glUniform3f(uniforms[name],n1, n2, n3);
	return *this;
}

Shader & Shader::setUniform(const char * name, float n1, float n2, float n3, float n4){
	glUniform4f(uniforms[name],n1, n2, n3, n4);
	return *this;
}

GLuint Shader::getUniform(const char * name){
	return uniforms[name];
}

//################# SHADER MANAGER #################
#include "lib/sdl.hpp"


void ShaderManager::clear(){
	shaders.clear();
}

Shader& ShaderManager::operator [](const char* n)
{
	return shaders[n];
}

ShaderManager::~ShaderManager(){
	ShaderManager::clear();
}

void ShaderManager::setP(glm::mat4* projection){
	for(auto iter = shaders.begin(); iter!=shaders.end(); iter++){
		iter->second.select();
		iter->second.setP(projection);
	}
}

void ShaderManager::setV(glm::mat4* view){
	for(auto iter = shaders.begin(); iter!=shaders.end(); iter++){
		iter->second.select();
		iter->second.setV(view);
	}
}

void ShaderManager::setTime(float n){
	for(auto iter = shaders.begin(); iter!=shaders.end(); iter++){
		iter->second.select();
		iter->second.setTime(n);
	}
}

void ShaderManager::setProportions(float n){
	for(auto iter = shaders.begin(); iter!=shaders.end(); iter++){
		iter->second.select();
		iter->second.setProportions(n);
	}
}

void ShaderManager::setResolution(float w, float h){
	for(auto iter = shaders.begin(); iter!=shaders.end(); iter++){
		iter->second.select();
		iter->second.setResolution(w,h);
	}
}
