#pragma once

#include <vector>

#include "lib/sdl.h"
#include "lib/opengl.h"

#include "frame.h"

class FrameManager{
private:
    int currentframe=0;
    std::vector<Frame*>frames;
    void updateIndexes();
public:
    int getFrameCount();
    void createFrame(int w, int h);
    void duplicateFrame();
    void removeFrame();
    void clearFrames();
    void removeFrame(int n);
    void selectFrame(int n);
    Frame * getFrame(int index);
    Frame * getCurrentFrame();
    int getCurrentFrameIndex();
    ~FrameManager();
};
