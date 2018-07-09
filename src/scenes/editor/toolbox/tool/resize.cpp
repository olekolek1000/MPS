#include "resize.hpp"
#include "../../drawer.hpp"
#include "../toolbox.hpp"
#include "../../editor.hpp"

void Resize::init(){
    iconName = "resize";

    Frame * frame = scene->frameMan.getCurrentFrame();
    box_width.init(scene).setMinValue(1).setMaxValue(8192).setValue(frame->getWidth());
    box_height.init(scene).setMinValue(1).setMaxValue(8192).setValue(frame->getHeight());

    but_apply.init(scene).setText("Apply", scene->a.font32, 255, 255, 255);
}

void Resize::update(){
    box_width.update();
    box_height.update();
    but_apply.update();
}

bool Resize::pushEvent(SDL_Event * evt){
    bool used=false;

    if(box_width.pushEvent(evt)){
		used=true;
	}
    if(box_height.pushEvent(evt)){
		used=true;
	}
    if(but_apply.pushEvent(evt)){
        if(but_apply.isClicked()){
            scene->drawer.historyClear();
            scene->frameMan.getCurrentFrame()->resize(box_width.getValue(), box_height.getValue());
            scene->changeFrame(scene->frameMan.getCurrentFrameIndex());
            Float2 p = scene->drawer.getCameraPosition();
            scene->drawer.setCameraPosition(p.x, p.y);
        }
        used=true;
    }

    return used;
}


void Resize::render(float alpha){
	box_width.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()).setSize(toolbox->getWidth(),75).render(alpha);
    box_height.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()+75).setSize(toolbox->getWidth(),75).render(alpha);
    but_apply.setPosition(toolbox->getSettingsX(),toolbox->getSettingsY()+150).setSize(toolbox->getWidth(),40).render(alpha);
}

void Resize::select(){
    toolbox->setSettingsHeight(190);
}
