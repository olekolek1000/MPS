#include "framemanager.h"

typedef unsigned int uint;

int FrameManager::getFrameCount(){
    return (int)frames.size();
}

void FrameManager::createFrame(int w, int h){
    Frame * frame = new Frame;
    frame->load(w,h);
    if(getCurrentFrameIndex()<getFrameCount()){
        frames.insert(frames.begin()+getCurrentFrameIndex()+1, frame);
    }
    else{
        frames.push_back(frame);
    }

    updateIndexes();
}

void FrameManager::duplicateFrame(){
    Frame * frame = new Frame;
    frame->load(getCurrentFrame()->getWidth(),getCurrentFrame()->getHeight());
    SDL_BlitSurface(getCurrentFrame()->overlay,NULL,frame->overlay,NULL);
    frame->needUpdate=true;
    if(getCurrentFrameIndex()<getFrameCount()){
        frames.insert(frames.begin()+getCurrentFrameIndex()+1, frame);
    }
    else{
        frames.push_back(frame);
    }
    updateIndexes();
}

void FrameManager::selectFrame(int n){
    int c;
    if(n<0){
        c=0;
    }
    else if(n>=getFrameCount()){
        c=getFrameCount()-1;
    }
    else{
        c=n;
    }

    currentframe=c;
}

Frame * FrameManager::getCurrentFrame(){
    return frames[currentframe];
}

int FrameManager::getCurrentFrameIndex(){
    return currentframe;
}

void FrameManager::removeFrame(int n){
    if(getFrameCount()>1){
        int c;
        if(n<0){
            c=0;
        }
        else if(n>=getFrameCount()){
            c=getFrameCount();
        }
        else{
            c=n;
        }

        delete frames[c];
        frames.erase(frames.begin()+c);

        updateIndexes();
    }
}

void FrameManager::removeFrame(){
    if(getFrameCount()>1){
        delete frames[getCurrentFrameIndex()];
        frames.erase(frames.begin()+getCurrentFrameIndex());

        updateIndexes();
    }
}

Frame * FrameManager::getFrame(int index){
    if(index>=0&&index<(int)frames.size()){
        return frames[index];
    }
    else{
        return NULL;
    }
}

void FrameManager::clearFrames(){
    int size = getFrameCount();
    for(int i=0;i<size;i++){
        delete frames[i];
    }
    frames.clear();
	currentframe=0;
}

FrameManager::~FrameManager(){
    clearFrames();
}

void FrameManager::updateIndexes(){
    for(uint i=0;i<frames.size();i++){
        frames[i]->index = i;
    }
}
