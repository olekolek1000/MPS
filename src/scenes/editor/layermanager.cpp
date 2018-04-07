#include "layermanager.hpp"
#include "editor.hpp"
#include <sstream>

void LayerCell::init(Layer *layer){
    this->layer = layer;
}

LayerCell::~LayerCell(){
}

void LayerManager::updateList(){
    cells.clear();
    cells.resize(currentFrame->getLayerCount());
    for (int i = 0; i < currentFrame->getLayerCount(); i++){
        cells[i].init(currentFrame->getLayer(i));
        cells[i].but_set.init(scene).setSize(30, 40).setTexture(&scene->thMan["ls/select"]);
        cells[i].but_visibility.init(scene).setSize(40,40).setBackgroundVisible(false);
        Layer * l = currentFrame->getLayer(i);
        if(l->isVisible()){
            cells[i].but_visibility.setTexture(&scene->thMan["ls/visible"]);
        }
        else{
            cells[i].but_visibility.setTexture(&scene->thMan["ls/invisible"]);
        }
        cells[i].but_remove.init(scene).setSize(40,40).setTexture(&scene->thMan["fs/removeframe"]);
        std::stringstream ss;
        ss<<"Layer "<<l;
        cells[i].text_index.init(scene).changeText(ss.str(), scene->a.font24, 0, 0, 0);
    }
}

void LayerManager::setCurrentFrame(Frame *frame){
    if (currentFrame != frame){
        currentFrame = frame;
        updateList();
    }
}

void LayerManager::init(sceneEditor *scene){
    this->scene = scene;
    but_addLayer.init(scene).setPosition(200,20).setSize(40,40).setTexture(&scene->thMan["fs/newframe"]);
    text_buildinprogress.init(scene).setPosition(250,20).changeText("Layer support - Build in progress (!)",scene->a.font16,0,0,0);
}

bool LayerManager::pushEvent(SDL_Event *evt){
    bool used = false;
    
    if(but_addLayer.pushEvent(evt)){
        if(but_addLayer.isClicked()){
            currentFrame->selectLayer(currentFrame->createLayer());
            updateList();
        }
        used=true;
    }
    if(used==false){
        for(uint i = 0; i < cells.size(); i++){
            if(cells[i].but_set.pushEvent(evt)){
                if(cells[i].but_set.isClicked()){
                    scene->drawer.historyClear();
                    currentFrame->selectLayer(i);
                    scene->drawer.historyCreateSnapshot();
                }
                used = true;
            }
            if(cells[i].but_visibility.pushEvent(evt)){
                if(cells[i].but_visibility.isClicked()){
                    Layer * l = currentFrame->getLayer(i);
                    l->setVisibility(!l->isVisible());
                    if(l->isVisible()){
                        cells[i].but_visibility.setTexture(&scene->thMan["ls/visible"]);
                    }
                    else{
                        cells[i].but_visibility.setTexture(&scene->thMan["ls/invisible"]);
                    }
                }
                used=true;
            }
            if(cells[i].but_remove.pushEvent(evt)){
                if(cells[i].but_remove.isClicked()){
                    currentFrame->removeLayer(i);
                    updateList();
                    break;
                }
                used=true;
            }
        }
    }
    return used;
}

void LayerManager::update(){

}

void LayerManager::render(){
    for(int i = 0; i < (int)cells.size(); i++){
        int pY = 70+i*40;

        if(i == currentFrame->getSelectedLayerIndex()){
            cells[i].but_set.setPosition(200, pY).render();
        }
        else{
            cells[i].but_set.setPosition(190, pY).render();
        }
        cells[i].but_visibility.setPosition(230, pY).render();
        cells[i].but_remove.setPosition(270, pY).render();
        cells[i].text_index.setPosition(310, pY).render();

        but_addLayer.render();
    }
    text_buildinprogress.render();
}