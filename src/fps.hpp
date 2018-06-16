#pragma once
class Fps{
public:
    unsigned short fps;
    unsigned int tick_prev;
    unsigned int frame;
    bool frameskip;

    Fps();
    void start(int fps);
    void end();
};
