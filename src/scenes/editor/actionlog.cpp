#include "gui/text.hpp"
#include "actionlog.hpp"
#include "editor.hpp"
#include "lib/sdl.hpp"
#include "app.hpp"


void ActionLog::init(sceneEditor * scene){
    this->scene = scene;
}
ActionLog::~ActionLog(){
    clear();
}

void ActionLog::clear(){
    for(unsigned int i=0; i<cells.size(); i++){
        delete cells[i].text;
    }
    cells.clear();
}

void ActionLog::addMessage(const char * n, float timemultiplier){
    ActionCell cell;
    cell.text = new GuiText(scene, n, scene->a.font16, 255, 255, 255);
    cell.text->setAlign(2, 1);
    cell.starttime = (int)SDL_GetTicks();
    cell.timemultiplier = timemultiplier;
    cells.push_back(cell);
    if(cells.size()>10){
        delete cells[0].text;
        cells.erase(cells.begin());
    }
}  


void ActionLog::render(){
    int time = (int)SDL_GetTicks();
    float alpha, x;
    for(unsigned int i=cells.size()-1; i>0; i--){
        ActionCell * cell = &cells[i];
        GuiText * text = cell->text;
        if(time-cell->starttime<80){
            x = scene->a.getAreaWidth() + text->getWidth() * (1.0-(time-cell->starttime)/80.0f);
        }
        else{
            x = scene->a.getAreaWidth();
        }
        text->setPosition(x, scene->a.getAreaHeight()-scene->frameselector.getHeight()-20-((int)cells.size()*20)+i*20);
        alpha = 1.5-(time-cell->starttime)/2500.0f/cell->timemultiplier*1.5f;
        text->setAlpha(std::min(alpha*2.5, 1.0)).setBackgroundColor(0.0f,0.0f,0.0f,alpha/3.0f);
        text->render();

        if(cell->starttime+2500*cell->timemultiplier<time){
            delete cell->text;
            cells.erase(cells.begin()+i);
        }
    }
}

bool ActionLog::pushGlobalEvent(GlobalEvent evt){
    bool used=false;

    switch(evt){
        case GlobalEvent::GuiChange:{
            for(unsigned int i=0; i<cells.size(); i++){
                cells[i].text->changeText(cells[i].text->getText(), scene->a.font16, 255, 255, 255);
            }
            used=true;
            break;
        }
        default:break;
    }

    return used;
}