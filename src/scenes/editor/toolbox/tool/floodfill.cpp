#include "floodfill.hpp"
#include "../../editor.hpp"

#include <stack>

void FloodFill::init(){
    iconName = "floodfill";
}

void FloodFill::eventMouseMove(int x, int y){
    this->x=x;
    this->y=y;

}

void FloodFill::eventMouseDown(){
    oldcolor = scene->drawer.getPoint(this->x,this->y);
    if(oldcolor!= scene->drawer.getSelectedColor()){
        startX = this->x;
        startY = this->y;
        floodfill(this->x,this->y);
    }
    scene->drawer.historyCreateSnapshot();
}

void FloodFill::eventMouseUp(){

}


void FloodFill::floodfill(int posX, int posY){
    int count = 0;
    struct coordinate{
        int x, y;
    };
    std::stack<coordinate> drawqueue;
    drawqueue.push({x, y});
    while (!drawqueue.empty())
    {
        auto top = drawqueue.top();
        drawqueue.pop();
        if(top.x>=0 && top.y>=0 && top.x< scene->frameMan.getCurrentFrame()->getWidth() && top.y < scene->frameMan.getCurrentFrame()->getHeight() && oldcolor == scene->drawer.getPoint(top.x,top.y)){
            scene->drawer.drawPoint(top.x, top.y);
            count++;
            drawqueue.push({top.x, top.y + 1});
            drawqueue.push({top.x, top.y - 1});
            drawqueue.push({top.x + 1, top.y});
            drawqueue.push({top.x - 1, top.y});
        }
    }
    scene->actionlog.addMessage(std::string("Filled "+std::to_string(count)+" pixels.").c_str());
}


bool FloodFill::pushEvent(SDL_Event * evt){
    bool used=false;

    return used;
}
void FloodFill::update(){

}
void FloodFill::render(){

}
void FloodFill::select(){
    scene->toolbox.setSettingsHeight(0);
}
