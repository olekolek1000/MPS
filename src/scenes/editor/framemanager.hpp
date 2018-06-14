#pragma once

#include <vector>

#include "lib/sdl.hpp"
#include "lib/opengl.hpp"

#include "frame.hpp"

class FrameManager{
private:
    int currentframe=0;
    std::vector<Frame*>frames;
    void updateIndexes();
public:
    int getFrameCount();
    Frame * createFrame(int w, int h);
    void duplicateFrame();
    bool removeFrame();
    bool removeFrame(int n);
    void clearFrames();
    void selectFrame(int n);
    Frame * getFrame(int index);
    Frame * getCurrentFrame();
    int getCurrentFrameIndex();
    ~FrameManager();
};
