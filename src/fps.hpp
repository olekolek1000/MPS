#pragma once
class Fps{
public:
    int fps=0;
    int tick_prev=0;
    int frame=0;
    bool frameskip=false;

    Fps();
    void start(int fps);
    void end();
};
