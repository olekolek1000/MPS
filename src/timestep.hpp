#pragma once

#include "lib/sdl.hpp"
using namespace std;

float alphize(float alpha, float prev, float var);

class Timestep{
private:
    int frame=0;
    int fcheck=SDL_GetTicks();
    int loopnum=0;

    int currenttime = 0;
    float accumulator = 0.0;
    float delta = 1000.0/60.0;
    float alpha = 0.0;
    float time = 0.0;
    float speed = 1.0;

    void calculateAlpha();
public:
    Timestep();
    void setDelta(float n);
    void setRate(float n);
    float getAlpha();
    float getTime();
    void setSpeed(float n);
    float getSpeed();
    bool onUpdate();
	void reset();
};
