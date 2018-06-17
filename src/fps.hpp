#pragma once
class Fps{
public:
    short fps;
    int tick_prev;
    int frame;
    bool frameskip;

    Fps();
    void start(int fps);
    void end();
};
