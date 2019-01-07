#include "checkbox.hpp"
#include "../editor.hpp"
#include "lib/glm.hpp"
#include "transform.hpp"
#include "render/buffer.hpp"
#include "timestep.hpp"

struct GuiCheckbox::Pimpl{
    sceneEditor * s;
    int posX, posY;
    int size;
    glm::mat4 model;
    float scale = 1.0f;
    float scale_smooth = 1.0f;
    float scale_smooth_prev = 1.0f;
    bool hovered=false;
};

GuiCheckbox::GuiCheckbox(){
    p = new Pimpl;
}

GuiCheckbox::~GuiCheckbox(){
    delete p;
}

GuiCheckbox::GuiCheckbox(sceneEditor * s, int posX, int posY, int size){
    p = new Pimpl;
    init(s, posX, posY, size);
}

GuiCheckbox& GuiCheckbox::init(sceneEditor * s, int posX, int posY, int size){
    p->s=s;
    p->posX=posX;
    p->posY=posY; 
    p->size=size;
    return *this;
}

bool GuiCheckbox::pushEvent(SDL_Event * evt){
    bool used=false;

    if(evt->type==SDL_MOUSEMOTION){
        int mX=evt->motion.x*p->s->a.getAreaMultiplier();
        int mY=evt->motion.y*p->s->a.getAreaMultiplier();
        if(mX>=p->posX&&mY>=p->posY&&mX<p->posX+p->size&&mY<p->posY+p->size){
            p->hovered=true;
        }
        else{
            p->hovered=false;
        }
    }

    return used;
}

GuiCheckbox & GuiCheckbox::update(){
    p->scale_smooth_prev=p->scale_smooth;
    p->scale_smooth+= (p->scale-p->scale_smooth)*0.5f;
    if(p->hovered){
        p->scale=1.15f;
    }
    else{
        p->scale=1.0f;
    }
    return *this;
}

GuiCheckbox & GuiCheckbox::render(float alpha){
    
    sceneEditor * s = p->s;
    xReset(&p->model);
    xTranslate(&p->model, p->posX, p->posY);
    xScale(&p->model, p->size, p->size);
    s->shGui.select().setM(&p->model);
    s->thMan["checkbox"].select();
    s->a.square_vert->bind().attrib(0,2,GL_FLOAT).draw(GL_TRIANGLES);
    xReset(&p->model);
    float scale = alphize(alpha, p->scale_smooth_prev, p->scale_smooth);
    xTranslate(&p->model, p->posX-p->size*(scale-1.0f)/2.0f, p->posY-p->size*(scale-1.0f)/2.0f);
    xScale(&p->model, p->size*scale, p->size*scale);
    s->shGui.setM(&p->model);
    s->thMan["icons/checkbox"].select(); 
    s->a.square_vert->draw(GL_TRIANGLES);
    return *this;
}