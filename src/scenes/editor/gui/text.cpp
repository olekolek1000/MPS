#include "text.h"
#include "lib/opengl.h"
#include "transform.h"
#include "../editor.h"

#include "render/func.h"

int GuiText::getWidth(){
    return width;
}
int GuiText::getHeight(){
    return height;
}
float GuiText::getProportions(){
    return width/(float)height;
}

GuiText& GuiText::changeText(std::string text, TTF_Font * font, Uint8 r, Uint8 g, Uint8 b){
    this->text = text;
    if(loaded){
        free();
    }

    SDL_Surface * textsurf = TTF_RenderUTF8_Blended(font, text.c_str(), SDL_Color{r,g,b});
    TTF_SizeUTF8(font, text.c_str(), &width, &height);

    SDL_Surface * converted = SDL_ConvertSurfaceFormat(textsurf, SDL_PIXELFORMAT_ABGR8888, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    SDL_FreeSurface(textsurf);
    SDL_FreeSurface(converted);

    loaded=true;
	return *this;
}

void GuiText::construct(){
	model = new glm::mat4;
	glGenTextures(1,&texture);
}

GuiText::GuiText(){
	construct();
}

GuiText& GuiText::init(sceneEditor * scene){
    this->scene = scene;
	return *this;
}

GuiText::GuiText(sceneEditor * scene){
    this->scene = scene;
	construct();
}

GuiText::GuiText(sceneEditor * scene, std::string text, TTF_Font * font, Uint8 r, Uint8 g, Uint8 b){
    this->scene = scene;
	construct();
    changeText(text, font, r, g, b);
}

void GuiText::free(){
    if(loaded){
        loaded=false;
    }
}

GuiText::~GuiText(){
	delete (glm::mat4*)model;
    glDeleteTextures(1, &texture);
}

GuiText& GuiText::setPosition(int x, int y){
    this->posX=x;
    this->posY=y;
	return *this;
}

GuiText& GuiText::setXAlign(char n){
	alignX=n;
	return *this;
}

GuiText& GuiText::setYAlign(char n){
	alignY=n;
	return *this;
}

GuiText& GuiText::setAlign(char x, char y){
	setXAlign(x);
	setYAlign(y);
	return *this;
}


GuiText& GuiText::setRotation(float angle){
    this->angle = angle;
	return *this;
}

GuiText& GuiText::render(){
    if(loaded){
        int x,y;
		switch(alignX){
			case 0:{x=posX;break;}
			case 1:{x=posX-width/2;break;}
			case 2:{x=posX-width;break;}
			default:{x=0;break;}
		}
		switch(alignY){
			case 0:{y=posY;break;}
			case 1:{y=posY-height/2;break;}
			case 2:{y=posY-height;break;}
			default:{y=0;break;}
		}

        xReset((glm::mat4*)model);
        xTranslate((glm::mat4*)model, x, y);
        if(angle!=0.0){
            xRotate((glm::mat4*)model, angle/57.296);
        }
        xScale((glm::mat4*)model,width, height);
        scene->shGui.select();
        glBindTexture(GL_TEXTURE_2D, texture);
        scene->shGui.setM((glm::mat4*)model);
        scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
		scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
		rDraw(GL_TRIANGLES,scene->a.square_vert->getSize());
    }
	return *this;
}
