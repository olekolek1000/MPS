#pragma once

#include "lib/glm.hpp"
typedef unsigned int GLuint;

void xReset(glm::mat4 * model);
void xRotate(glm::mat4 * model, float angle);
void xScale(glm::mat4 * model, float x, float y);
void xTranslate(glm::mat4 * model, float x, float y);
void xCenter(glm::mat4 * model);
void xApplyMat(GLuint sh_var, glm::mat4 * mat);

void zReset(glm::mat4 * model);
void zRotateX(glm::mat4 * model, float angle);
void zRotateY(glm::mat4 * model, float angle);
void zRotateZ(glm::mat4 * model, float angle);
void zScale(glm::mat4 * model, float x, float y, float z);
void zTranslate(glm::mat4 * model, float x, float y, float z);

void sendM(GLuint *sh_m, glm::mat4*model);
void sendV(GLuint *sh_v, glm::mat4*view);
void sendP(GLuint *sh_p, glm::mat4*projection);
