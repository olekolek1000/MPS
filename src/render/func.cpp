#include "func.h"

void rDraw(GLenum mode, GLuint size){
	glDrawArrays(GL_TRIANGLES, 0, size);
} 