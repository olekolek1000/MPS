#include "button.hpp"
#include "lib/opengl.hpp"
#include "transform.hpp"
#include "../editor.hpp"
#include "text.hpp"

#include "lib/glm.hpp"

#include "render/func.hpp"

#include "timestep.hpp"

int GuiButton::getWidth(){
    return width;
}
int GuiButton::getHeight(){
    return height;
}
int GuiButton::getPosX(){
    return posX;
}
int GuiButton::getPosY(){
    return posY;
}

GuiButton & GuiButton::init(sceneEditor * scene){
    this->scene = scene;
    return *this;
}
GuiButton & GuiButton::init(sceneEditor * scene, int posX, int posY, int width, int height){
    this->scene=scene;
    setPosition(posX,posY);
    setSize(width,height);
    return *this;
}
GuiButton & GuiButton::setPosition(int posX, int posY){
    this->posX=posX;
    this->posY=posY;
    return *this;
}
GuiButton & GuiButton::setSize(int width, int height){
    this->width=width;
    this->height=height;
    return *this;
}

GuiButton & GuiButton::setBackgroundVisible(bool n){
    background = n;
    return *this;
}

bool GuiButton::pushEvent(SDL_Event * evt){
    bool used=false;
    switch(evt->type){
        case SDL_MOUSEBUTTONDOWN:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(evt->button.x*scene->a.getAreaMultiplier()>=posX&&evt->button.y*scene->a.getAreaMultiplier()>=posY&&evt->button.x*scene->a.getAreaMultiplier()<posX+width&&evt->button.y*scene->a.getAreaMultiplier()<posY+height){
                    used=true;
                    pressed=true;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(pressed){
                    if(evt->button.x*scene->a.getAreaMultiplier()>=posX&&evt->button.y*scene->a.getAreaMultiplier()>=posY&&evt->button.x*scene->a.getAreaMultiplier()<posX+width&&evt->button.y*scene->a.getAreaMultiplier()<posY+height){
                        used=true;
                        clicked=true;
                    }
                    pressed=false;
                }
            }

            break;
        }
        case SDL_MOUSEMOTION:{
            if(evt->motion.x*scene->a.getAreaMultiplier()>=posX&&evt->motion.y*scene->a.getAreaMultiplier()>=posY&&evt->motion.x*scene->a.getAreaMultiplier()<posX+width&&evt->motion.y*scene->a.getAreaMultiplier()<posY+height){
                hovered=true;
            }
            else{
                hovered=false;
            }
            if(pressed){
                used=true;
            }
            break;
        }
    }
    return used;
}

bool GuiButton::isClicked(){
    if(clicked){
        clicked=false;
        return true;
    }
    else{
        return false;
    }
}

bool GuiButton::isPressed(){
    return pressed;
}

GuiButton::GuiButton(){
	model = new glm::mat4;
}

GuiButton::~GuiButton(){
	delete (glm::mat4*)model;
    if(text!=NULL){
        delete text;
    }
}

GuiButton & GuiButton::update(){
    scale_smooth_prev=scale_smooth;
    scale_smooth+= (scale-scale_smooth)*0.5f;
    if(hovered){
        scale=1.15f;
    }
    else{
        scale=1.0f;
    }
    return *this;
}

GuiButton & GuiButton::render(float alpha){
    xReset((glm::mat4*)model);
    if(!pressed){
        xTranslate((glm::mat4*)model, posX,posY);
    }
    else{
        xTranslate((glm::mat4*)model, posX+2,posY+2);
    }
    xScale((glm::mat4*)model,width, height);

    scene->shGui.select();
    scene->shGui.setM((glm::mat4*)model);

    scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
	scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);

    if(background){
        scene->thMan["button"].select();
        scene->a.square_vert->draw(GL_TRIANGLES);
    }
    float scale = alphize(alpha, scale_smooth_prev, scale_smooth);
    if(texture!=NULL){
        texture->select();
        xReset((glm::mat4*)model);
        xTranslate((glm::mat4*)model, posX-width*(scale-1.0f)/2.0f+(pressed==1 ? 2 : 0), posY-height*(scale-1.0f)/2.0f+(pressed==1 ? 2 : 0));

        xScale((glm::mat4*)model, width*scale, height*scale);
        scene->shGui.setM((glm::mat4*)model);
        scene->a.square_vert->draw(GL_TRIANGLES);
    }
    if(text!=NULL){
        text->setBackgroundColor(0.0f,0.0f,0.0f,(scale-1.0f)*2.0f).setPosition(posX+width/2+(pressed==1 ? 2 : 0), posY+height/2+(pressed==1 ? 2 : 0)).render();
    }
    return *this;
}

GuiButton & GuiButton::setTexture(Texture * texture){
    this->texture = texture;
    return *this;
}

GuiButton & GuiButton::setText(std::string text, TTF_Font * font, char r, char g, char b){
    if(this->text!=NULL){
        delete this->text;
    }
    this->text = new GuiText(scene, text, font, r, g, b);
    this->text->setAlign(1,1);
    return *this;
}