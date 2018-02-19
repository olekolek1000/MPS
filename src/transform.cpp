#include "transform.h"
#include "lib/opengl.h"

glm::mat4 reset_value = glm::mat4(1.0);

void xReset(glm::mat4 * model)
{
    *model = reset_value;
}

void xRotate(glm::mat4 * model, float angle)
{
    *model = glm::rotate(*model,angle,glm::vec3(0.0f,0.0f,1.0f));
}

void xScale(glm::mat4 * model, float x, float y)
{
    *model = glm::scale(*model,glm::vec3(x,y,1.0f));
}

void xTranslate(glm::mat4 * model, float x, float y)
{
    *model = glm::translate(*model,glm::vec3(x,y,0.0f));
}

void xCenter(glm::mat4 * model){
	xTranslate(model,-0.5,-0.5);
}

void xApplyMat(GLuint sh_var, glm::mat4 * mat)
{
    glUniformMatrix4fv(sh_var,1,GL_FALSE,(const float*)mat);
}

void zReset(glm::mat4 * model)
{
    *model=reset_value;
}
void zRotateX(glm::mat4 * model, float angle)
{
    *model = glm::rotate(*model,angle,glm::vec3(1.0f,0.0f,0.0f));
}
void zRotateY(glm::mat4 * model, float angle)
{
    *model = glm::rotate(*model,angle,glm::vec3(0.0f,1.0f,0.0f));
}
void zRotateZ(glm::mat4 * model, float angle)
{
    *model = glm::rotate(*model,angle,glm::vec3(0.0f,0.0f,1.0f));
}
void zScale(glm::mat4 * model, float x, float y, float z)
{
    *model = glm::scale(*model,glm::vec3(x,y,z));
}
void zTranslate(glm::mat4 * model, float x, float y, float z)
{
    *model = glm::translate(*model,glm::vec3(x,y,z));
}

void sendM(GLuint *sh_m, glm::mat4*model)
{
	glUniformMatrix4fv(*sh_m, 1, GL_FALSE,(const GLfloat*)&model[0][0]);
}

void sendV(GLuint *sh_v, glm::mat4*view)
{
	glUniformMatrix4fv(*sh_v, 1, GL_FALSE,(const GLfloat*)&view[0][0]);
}

void sendP(GLuint *sh_p, glm::mat4*projection)
{
	glUniformMatrix4fv(*sh_p, 1, GL_FALSE,(const GLfloat*)&projection[0][0]);
}
