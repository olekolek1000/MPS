#include "framebuffermanager.h"
#include "lib/opengl.h"
#include "error.h"

void Framebuffer::generate(){
    if(generated){
        Framebuffer::free();
        initialized=false;
        generated=false;
    }

    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &depthbuffer);
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Framebuffer::generateBind(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        error("Error creating framebuffer!");
    }
}

void Framebuffer::create(int w, int h, int fbspace, int fbfiltering){
    #ifdef USE_FRAMEBUFFER
    Framebuffer::generate();
    Framebuffer::setSize(w, h, fbspace, fbfiltering);
    Framebuffer::generateBind();

    generated=true;
    initialized=true;
    #else
    error("Framebuffer support not compiled!");
    #endif
}

void Framebuffer::create(){
    #ifdef USE_FRAMEBUFFER
    Framebuffer::generate();
    generated=true;
    initialized=false;
    #else
    error("Framebuffer support not compiled!");
    #endif
}

void Framebuffer::free(){
    if(generated){
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteTextures(1, &texture);
        generated=false;
    }
}
Framebuffer::~Framebuffer(){
    Framebuffer::free();
}
void Framebuffer::select(){
    if(!generated){
        error("error: Selecting not generated framebuffer");
    }
    if(!initialized){
        error("error: Selecting uninitialized framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glViewport(0,0,width, height);
}

void Framebuffer::select(bool clearColor, bool clearDepth, float r, float g, float b, float a){
    Framebuffer::select();
    glClearColor(r,g,b,a);
    if(clearColor&&!clearDepth){
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if(!clearColor&&clearDepth){
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    else if(clearColor&&clearDepth){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
}


GLuint Framebuffer::getTexture(){
    return texture;
}
int Framebuffer::getWidth(){
    return width;
}
int Framebuffer::getHeight(){
    return height;
}
void Framebuffer::setSize(int w, int h, int fbspace, int fbfiltering){
    if(w<=0||h<=0){
        error("Bad framebuffer size (below zero)");
    }
    width=w;
    height=h;

    glBindTexture(GL_TEXTURE_2D, texture);
    if(fbspace==FB_RGB){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    else if(fbspace==FB_RGBA){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    else{
        error("Bad framebuffer space (can be only RGB or RGBA)");
    }

    if(fbfiltering == FBFILTERING_NEAREST){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else if(fbfiltering == FBFILTERING_LINEAR){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    if(!initialized){
        Framebuffer::generateBind();
        initialized=true;
    }
}
float Framebuffer::getProportions(){
    return (float)width/(float)height;
}

void FramebufferManager::clear(){
	framebuffers.clear();
}

Framebuffer& FramebufferManager::operator [](const char* n){
	return framebuffers[n];
}

FramebufferManager::~FramebufferManager(){
	FramebufferManager::clear();
}

