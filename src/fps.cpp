#include "fps.hpp"

#include "lib/sdl.hpp"

Fps::Fps(){
    tick_prev = (int)SDL_GetTicks();
}

void Fps::start(int fps){
    this->fps=fps;
}

void Fps::end(){
    if(fps>0){
        tick_prev += (1000.0/fps);
        frame++;
        frameskip=false;
        if(tick_prev>(int)SDL_GetTicks()){ 
            SDL_Delay(1000.0/fps);
        }
        if(tick_prev<(int)SDL_GetTicks()-40){
            if(frame%2==0){
                frameskip=true;
            } 
        }
        if(tick_prev<(int)SDL_GetTicks()-1000){
            tick_prev=(int)SDL_GetTicks()-500;
        }
    }
}
