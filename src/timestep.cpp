#include "timestep.hpp"

float alphize(float alpha, float prev, float var){
    return var*alpha + prev*(1.0-alpha);
}
void Timestep::calculateAlpha(){
    alpha = accumulator/delta;
}
Timestep::Timestep(){
    currenttime = SDL_GetTicks();
}
void Timestep::setDelta(float n){
    delta = n;
}
void Timestep::setRate(float n){
    setDelta(1000.0/n);
}
float Timestep::getAlpha(){
    return alpha;
}
float Timestep::getTime(){
    return time;
}
void Timestep::setSpeed(float n){
    speed = n;
}
float Timestep::getSpeed(){
    return speed;
}
bool Timestep::onUpdate(){
    int newtime = SDL_GetTicks();
    int frametime = newtime - currenttime;
    time+=frametime/1000.0;
    currenttime = newtime;
    accumulator+=(float)frametime*speed;
    calculateAlpha();
    if(accumulator>=delta){
        accumulator-=delta;
        loopnum++;

        if(loopnum>3){//cannot keep up!
            reset();
            loopnum=0;
            return true;
        }

        return true;
    }
    else{
        loopnum=0;
        return false;
    }
}
void Timestep::reset(){
	currenttime=SDL_GetTicks();
	accumulator=0.0;
}