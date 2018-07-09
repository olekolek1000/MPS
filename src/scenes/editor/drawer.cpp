#include "drawer.hpp"
#include "editor.hpp"
#include "pixel.hpp"
#include "transform.hpp"
#include "error.hpp"

#include "render/func.hpp"
#include "render/buffer.hpp"

#include "gui/text.hpp"

#include <sstream>

void Drawer::historyCreateSnapshot(){
    if(historypos<(int)history.size()){
        for(int i=historypos+1;i<(int)history.size();i++){
            delete history[i];
        }
        history.erase(history.begin()+historypos+1,history.end());
    }

    HistoryCell * cell = new HistoryCell;
    cell->create(currentFrame->getSelectedLayer()->getCanvas());
 
    history.push_back(cell);

    while((int)history.size()>historylimit){
        delete history[0];
        history.erase(history.begin());  
    }

    historypos=history.size()-1;
}

void Drawer::historyUpdate(){
    HistoryCell * cell = history[historypos];
    if(currentFrame->getWidth()!=cell->getWidth()||currentFrame->getHeight()!=cell->getHeight()){
        error("Different undo image sizes, this option is still unsupported, SORRY :/");
    }
    else{
        memcpy(currentFrame->getSelectedLayer()->getCanvas()->pixels,cell->getRaw(),cell->getRawSize());
        currentFrame->getSelectedLayer()->forceUpdate();
    }
}

void Drawer::historyUndo(){
    if(historypos>0){ 
        historypos--;
        scene->actionlog.addMessage("Undo"); 
        historyUpdate();
    } 
    else{
        scene->actionlog.addMessage("Nothing to undo! :(");
    } 
} 
void Drawer::historyRedo(){
    if(historypos<(int)history.size()-1){
        historypos++;
        scene->actionlog.addMessage("Redo");
        historyUpdate();
    }
    else{ 
        scene->actionlog.addMessage("Nothing to redo! :(");
    }
}

void Drawer::historyClear(){
    for(int i=0;i<(int)history.size();i++){
        delete history[i];
    }
    history.clear();
    historypos=0;
}


void Drawer::activeClear(){
    SDL_FillRect(activeOverlay, NULL, 0);
    activePreviewAlpha(1.0);
    activeNeedUpdate=true;
}

void Drawer::activeBlit(float alpha){
    if(alpha>0.9){
        SDL_BlitSurface(activeOverlay,NULL,currentFrame->getSelectedLayer()->getCanvas(),NULL);
    }
    else{
        SDL_SetSurfaceAlphaMod(activeOverlay, glm::clamp((int)(alpha*255.0),0,255));
        SDL_BlitSurface(activeOverlay,NULL,currentFrame->getSelectedLayer()->getCanvas(),NULL);
    }

    currentFrame->getSelectedLayer()->forceUpdate();
    activeClear();
}

void Drawer::activePreviewAlpha(float alpha){
    activealpha = alpha;
}

void Drawer::updateBorders(){
    float alpha = scene->thMan.config.getvarF("drawerBorderAlpha");
	float r = scene->thMan.config.getvarF("drawerBorderColorR");
	float g = scene->thMan.config.getvarF("drawerBorderColorG");
	float b = scene->thMan.config.getvarF("drawerBorderColorB");
	{
		float sLeft,sRight,sUp,sDown;
		float size = scene->thMan.config.getvarF("drawerBorderSize");
		if(currentFrame==NULL){
			sLeft=size;
			sRight=size;
			sUp=size;
			sDown=size;
		}
		else{
			float prop = currentFrame->getProportions();
			sLeft=size/prop;
			sRight=size/prop;
			sUp=size;
			sDown=size;
		}
		float coorddata[] = {
			0,0,
			-sLeft,0,
			0,1,
			-sLeft,0,
			-sLeft,1,
			0,1,
			
			1,0,
			1,1,
			1+sRight,0,
			1+sRight,0,
			1,1,
			1+sRight,1,
			
			0,1,
			0,1+sDown,
			1,1+sDown,
			0,1,
			1,1+sDown,
			1,1,
			
			0,0,
			0,0-sUp,
			1,0-sUp,
			0,0,
			1,0-sUp,
			1,0,
			//upleft
			0,0,
			-sLeft,0,
			0,-sUp,
			//upright
			1,0,
			1+sRight,0,
			1,-sUp,
			//downleft
			0,1,
			0,1+sDown,
			-sLeft,1,
			//downright
			1,1,
			1,1+sDown,
			1+sRight,1
		};
		bordercoordbuf.bind().setData(sizeof(coorddata),coorddata,GL_STATIC_DRAW);
	}
	{
		float colordata[] = {
			//left
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			r,g,b,alpha,
			//right
			r,g,b,alpha,
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			r,g,b,alpha,
			r,g,b,0,
			//down
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,alpha,
			//up
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,alpha,
			//upleft
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			//upright
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			//downleft
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0,
			//downright
			r,g,b,alpha,
			r,g,b,0,
			r,g,b,0
		};
		bordercolorbuf.bind().setData(sizeof(colordata),colordata,GL_STATIC_DRAW);
	}
}

void Drawer::reloadTextures(){
	updateBorders();
}

void Drawer::init(sceneEditor * scene){
    this->scene = scene;
    this->toolbox = &scene->toolbox;

    cameraX=0;
    cameraY=0;
    cameraZoom=0;
    cameraZoomAlpha=0;
    cameraZoomSmooth=0;
    cameraZoomSmoothPrev=0;
    cameraRotSmooth={1,1};
    
    textAngle = new GuiText;
    textAngle->init(scene);
	
    glGenTextures(1,&activeOverlayTex);
	updateBorders();
}

Drawer::~Drawer(){
    historyClear();
    glDeleteTextures(1,&activeOverlayTex);
    SDL_FreeSurface(activeOverlay);
    delete textAngle;
}

#include <iostream>
void Drawer::setCurrentFrame(Frame * frame){
    this->currentFrame = frame;
    if(activeOverlay == NULL){
        activeOverlay = SDL_CreateRGBSurface(0, frame->getWidth(), frame->getHeight(), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    }
    else{
        if(activeOverlay->w!=frame->getWidth()||activeOverlay->h!=frame->getHeight()){
            SDL_FreeSurface(activeOverlay);
            activeOverlay = SDL_CreateRGBSurface(0, frame->getWidth(), frame->getHeight(), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        }
    }
    SDL_SetSurfaceBlendMode(activeOverlay, SDL_BLENDMODE_BLEND);

    historyClear();
    historyCreateSnapshot();
	updateBorders();
}

void Drawer::setColor(Uint8 r, Uint8 g, Uint8 b){
    this->r=r;
    this->g=g;
    this->b=b;
    color = SDL_MapRGB(activeOverlay->format, r,g,b);
}

int Drawer::getSelectedColor(){
    return color;
}

void Drawer::activeDrawPoint(int x, int y){
    if(x>=0&&y>=0&&x<activeOverlay->w&&y<activeOverlay->h){
        putpixel(activeOverlay, x, y, color);
    }
    activeNeedUpdate=true;
}

void Drawer::activeDrawRectangle(int x, int y, int w, int h){
    SDL_Rect rect;
    if(w>=0){
        rect.x=x;rect.w=w;
    }
    else{
        rect.x=x+w;rect.w=-w;
    }
    if(h>=0){
        rect.y=y;rect.h=h;
    }
    else{
        rect.y=y+h;rect.h=-h;
    }
    SDL_FillRect(activeOverlay, &rect, color);

    activeNeedUpdate=true;
}

void Drawer::activeDrawLine(int startX, int startY, int endX, int endY, int thickness){
    int nowX,nowY;
    int iterations = sqrt(pow(endX-startX,2)+pow(endY-startY,2))+1;
    int xx,yy;
    float delta;
    int width = currentFrame->getWidth();
    int height = currentFrame->getHeight();
    
    for(int i=0; i<iterations; i++){
        delta = i/(float)iterations;
        nowX = round(startX*(delta) + endX*(1.0-delta));
        nowY = round(startY*(delta) + endY*(1.0-delta));

        if(thickness==1){
            activeDrawPoint(nowX, nowY);
        }
        else{
            for(int y=0;y<=thickness;y++){
                yy=nowY+y-thickness/2;
                for(int x=0;x<=thickness;x++){
                    xx=nowX+x-thickness/2;
                    if( xx>=0&&yy>=0&&xx<width&&yy<height&&getpixel(activeOverlay, xx, yy) != (uint)color && sqrt(pow(x-thickness/2,2)+pow(y-thickness/2,2))<(thickness+0.25)/2.0){
                        activeDrawPoint(xx, yy);
                    }
                }
            }
        }
    }
    activeNeedUpdate=true;
}

void Drawer::drawPoint(int x, int y){
    SDL_Surface * canvas = currentFrame->getSelectedLayer()->getCanvas();
    if(x>=0&&y>=0&&x<currentFrame->getWidth()&&y<currentFrame->getHeight()){
        if(erase){
            putpixel(canvas, x, y, 0);
        }
        else{
            putpixel(canvas, x, y, color);
        }
    }
    currentFrame->getSelectedLayer()->forceUpdate();
}

void Drawer::drawRectangle(int x, int y, int w, int h){
    SDL_Rect rect;
    rect.x=x;rect.y=y;rect.w=w;rect.h=h;
    SDL_FillRect(currentFrame->getSelectedLayer()->getCanvas(), &rect, color);

    currentFrame->getSelectedLayer()->forceUpdate();
}

void Drawer::drawLine(int startX, int startY, int endX, int endY, int thickness){
    int nowX,nowY,xx,yy;
    int iterations = sqrt(pow(endX-startX,2)+pow(endY-startY,2))+1;
    float delta;
    for(int i=0; i<iterations; i++){
        delta = i/(float)iterations;
        nowX = round(startX*(delta) + endX*(1.0-delta));
        nowY = round(startY*(delta) + endY*(1.0-delta));

        if(thickness==1){
            drawPoint(nowX, nowY);
        }
        else{
            for(int y=0;y<=thickness;y++){
                yy=nowY+y-thickness/2;
                for(int x=0;x<=thickness;x++){
                    xx=nowX+x-thickness/2;
                    if( getpixel(activeOverlay, xx, yy) != (uint)color && sqrt(pow(x-thickness/2,2)+pow(y-thickness/2,2))<(thickness+0.25)/2.0){
                        drawPoint(xx, yy);
                    }
                }
            }
        }
    }
    currentFrame->getSelectedLayer()->forceUpdate();
}


int Drawer::getPoint(int x, int y){
    if(x>=0&&y>=0&&x<currentFrame->getWidth()&&y<currentFrame->getHeight()){
        return getpixel(currentFrame->getSelectedLayer()->getCanvas(), x, y);
    }
    return 0;
}

void Drawer::setErase(bool n){
    erase=n;
}

bool Drawer::pushEvent(SDL_Event * evt){
    switch(evt->type){
        case SDL_KEYDOWN:{
            switch(evt->key.keysym.sym){
                case SDLK_SPACE:{
                    if(ghostopacity>0){
                        ghostopacity=0.0;
                    }
                    else{
                        ghostopacity=0.4;
                    }
                    break;
                }
                case SDLK_z:{
                    if(evt->key.keysym.mod & KMOD_LCTRL){
                        historyUndo();
                    }
                    break;
                }
                case SDLK_y:{
                    if(evt->key.keysym.mod & KMOD_LCTRL){
                        historyRedo();
                    }
                    break;
                }

                case SDLK_1:{setZoomPixelPerfect(1);break;}
                case SDLK_2:{setZoomPixelPerfect(2);break;}
                case SDLK_3:{setZoomPixelPerfect(3);break;}
                case SDLK_4:{setZoomPixelPerfect(4);break;}
                case SDLK_5:{setZoomPixelPerfect(8);break;}
                case SDLK_6:{setZoomPixelPerfect(16);break;}
                case SDLK_7:{setZoomPixelPerfect(24);break;}
                case SDLK_8:{setZoomPixelPerfect(32);break;}
                case SDLK_9:{setZoomPixelPerfect(64);break;}

                case SDLK_KP_1:{setZoomPixelPerfect(1);break;}
                case SDLK_KP_2:{setZoomPixelPerfect(2);break;}
                case SDLK_KP_3:{setZoomPixelPerfect(3);break;}
                case SDLK_KP_4:{setZoomPixelPerfect(4);break;}
                case SDLK_KP_5:{setZoomPixelPerfect(8);break;}
                case SDLK_KP_6:{setZoomPixelPerfect(16);break;}
                case SDLK_KP_7:{setZoomPixelPerfect(24);break;}
                case SDLK_KP_8:{setZoomPixelPerfect(32);break;}
                case SDLK_KP_9:{setZoomPixelPerfect(64);break;}
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                toolbox->getCurrentTool()->eventMouseDown();
            }
            if(evt->button.button==SDL_BUTTON_RIGHT){
                dragging=true;
                dragStart.x = evt->button.x;
				dragStart.y = evt->button.y;
                dragCameraStart = getCameraPosition();
            }
            if(evt->button.button==SDL_BUTTON_MIDDLE){
                int pixel = getPoint(drawX, drawY);
                Uint8 r,g,b;
                SDL_GetRGB(pixel, currentFrame->getSelectedLayer()->getCanvas()->format, &r,&g,&b);
                scene->colorselector.setColor(r,g,b);
                scene->actionlog.addMessage("Picked color.");
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                toolbox->getCurrentTool()->eventMouseUp();
            }
            if(evt->button.button==SDL_BUTTON_RIGHT){
                dragging=false;
                dragEnd.x = evt->button.x;
				dragEnd.y = evt->button.y;
            }
            break;
        }
        case SDL_MOUSEMOTION:{

            mouseX=evt->motion.x;
            mouseY=evt->motion.y;
			

            if(!dragging){
				if(scene->a.isPressed(SDL_SCANCODE_LSHIFT)){
					if(rotating==false){
						rotating=true;
						rotationStart = {(float)mouseX,(float)mouseY};
					}
					int centerX = rotationStart.x-mouseX;
					int centerY = rotationStart.y-mouseY;
					
					float angle = atan2(centerX, centerY)-1.57;
					animationSpeed=animationSpeedDefault;
					cameraRot.x = cos(angle);
					cameraRot.y = sin(angle);
				}
				else{
					rotating=false;
				}
				glm::vec3 win = glm::vec3(mouseX, scene->a.getWindowHeight()-mouseY,0.0);
				glm::vec4 viewport = glm::vec4(0,0,scene->a.getWindowWidth(), scene->a.getWindowHeight());
				glm::vec3 proj = glm::unProject(win, model, projection, viewport);
				drawX = proj.x*currentFrame->getWidth();
				drawY = proj.y*currentFrame->getHeight(); 
			
				toolbox->getCurrentTool()->eventMouseMove(drawX, drawY);
				
                if(drawX>=0&&drawY>=0&&drawX<currentFrame->getWidth()&&drawY<currentFrame->getHeight()){
                    scene->setCursor("crosshair");
                }
                else{
                    scene->setCursor("arrow");
                }
            }
            else{
                scene->setCursor("drag");
            }

            if(dragging){ 
                Float2 pos;

                pos.x = dragCameraStart.x + (mouseX - dragStart.x)/cameraZoomAlpha;
                pos.y = dragCameraStart.y + (mouseY - dragStart.y)/cameraZoomAlpha;

                setCameraPosition(pos.x, pos.y);
            } 

            break;
        }
        case SDL_MOUSEWHEEL:{
            int y = evt->wheel.y;
            if(y>0){
                zoomIn(y*0.28);
            }
            if(y<0){
                zoomOut(-y*0.28);
            }
        }
    }

    return true; 
}

void Drawer::setCameraPosition(float x, float y){
    if(x>0)x=0;
    if(y>0)y=0;
    if(x<-currentFrame->getWidth())x=-currentFrame->getWidth();
    if(y<-currentFrame->getHeight())y=-currentFrame->getHeight();
    cameraX=x;
    cameraY=y;
}

Float2 Drawer::getCameraPosition(){
    return Float2{cameraX,cameraY};
}

void Drawer::zoomIn(float n){
    cameraZoom = cameraZoom * (1.0+n);
	animationSpeed=animationSpeedDefault;
    std::stringstream ss;
    ss<<"Zoom: "<<cameraZoom*100<<"%";
    scene->actionlog.addMessage(ss.str().c_str(), 0.5f);
    updateViewport();
}

void Drawer::zoomOut(float n){
    cameraZoom = cameraZoom * (1.0-n);
	animationSpeed=animationSpeedDefault;
    std::stringstream ss;
    ss<<"Zoom: "<<cameraZoom*100<<"%";
    scene->actionlog.addMessage(ss.str().c_str(), 0.5f);
    updateViewport();
}

void Drawer::setZoomPixelPerfect(int n){
    std::stringstream ss;
    if(cameraZoomSmooth!=cameraZoomSmoothPrev){
        cameraZoomSmooth=cameraZoomSmoothPrev=cameraZoomAlpha=cameraZoom;
        cameraRotSmooth=cameraRotSmoothPrev=cameraRotAlpha=cameraRot;
        ss<<"Skipped animation. ";
    }
	cameraRot.x=0.0;
	cameraRot.y=1.0;
	cameraZoom = n;
	animationSpeed=animationSpeedDefault/1.5;
    ss<<"Pixel-Perfect Zoom: "<<cameraZoom*100<<"%";
    scene->actionlog.addMessage(ss.str().c_str());
    updateViewport();
}

void Drawer::updateViewport(){
    projection = glm::ortho(0.0f,(float)scene->a.getWindowWidth(),(float)scene->a.getWindowHeight(),0.0f);

	scene->shMan["overlay"].select().setUniform("OVERLAY_RES", currentFrame->getWidth(), currentFrame->getHeight());
    scene->shMan["overlaybg"].select().setUniform("OVERLAY_RES", currentFrame->getWidth(), currentFrame->getHeight()).setUniform("ZOOM", cameraZoomAlpha);
    scene->shMan["overlaygrid"].select().setUniform("OVERLAY_RES", currentFrame->getWidth(), currentFrame->getHeight()).setUniform("ZOOM", cameraZoomAlpha);
}


void Drawer::update(){
	
    cameraZoomSmoothPrev = cameraZoomSmooth;
    cameraZoomSmooth = cameraZoomSmooth + (cameraZoom-cameraZoomSmooth)*animationSpeed;
	
	cameraRotSmoothPrev = cameraRotSmooth;
	cameraRotSmooth.x = cameraRotSmooth.x + (cameraRot.x-cameraRotSmooth.x)*animationSpeed;
	cameraRotSmooth.y = cameraRotSmooth.y + (cameraRot.y-cameraRotSmooth.y)*animationSpeed;
	
	if(cameraZoom>80){
		zoomOut((cameraZoom-75)/2500.0);
	}
	if(cameraZoom<0.02){
		cameraZoom=0.02;
	}
}

void Drawer::render(float alpha){
    
    if(cameraZoomSmooth-cameraZoom<0.001&&cameraZoomSmooth-cameraZoom>-0.001){
        if(cameraZoomSmooth!=cameraZoom){
            cameraZoomSmooth=cameraZoom;
            cameraZoomAlpha=cameraZoom;
            updateViewport();
        }
    }
    else{
        cameraZoomAlpha = alphize(alpha, cameraZoomSmoothPrev,cameraZoomSmooth);
        updateViewport();
    } 

	xReset(&model);
	xTranslate(&model,round((scene->a.getWindowWidth()/2)+cameraX*cameraZoomAlpha),round((scene->a.getWindowHeight()/2)+cameraY*cameraZoomAlpha));
	xScale(&model, cameraZoomAlpha, cameraZoomAlpha);
	
	float rot = SDL_atan2(alphize(alpha, cameraRotSmoothPrev.x,cameraRotSmooth.x),alphize(alpha, cameraRotSmoothPrev.y,cameraRotSmooth.y));
	if(rot!=0.0f){
		xTranslate(&model,currentFrame->getWidth()/2,currentFrame->getHeight()/2);
		xRotate(&model, rot);
		xTranslate(&model,-currentFrame->getWidth()/2,-currentFrame->getHeight()/2);
	}
	
	xScale(&model, currentFrame->getWidth(), currentFrame->getHeight());

    scene->shMan["overlaybg"].select().setP(&projection).setM(&model);
	scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
	scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
	scene->a.square_vert->draw(GL_TRIANGLES);

    Shader & overlaysh = scene->shMan["overlay"];
    overlaysh.select().setP(&projection).setM(&model);

    for(int i=1;i<=ghostback;i++)
    {//ghosting back
        if(currentFrame->getIndex()-i>=0){
            if(currentFrame->getWidth()==scene->frameMan.getFrame(currentFrame->getIndex()-i)->getWidth()&&
                currentFrame->getHeight()==scene->frameMan.getFrame(currentFrame->getIndex()-i)->getHeight()
            ){
                overlaysh.setUniform("ALPHA",((ghostback-i+1)/(float)ghostback)*ghostopacity);
                scene->frameMan.getFrame(currentFrame->getIndex()-i)->getSelectedLayer()->bindTexture();
                scene->a.square_vert->draw(GL_TRIANGLES);
            }
        }
    } 
    for(int i=1;i<=ghostfront;i++)
    {//ghosting front
        if(currentFrame->getIndex()+i<scene->frameMan.getFrameCount()){
            if(currentFrame->getWidth()==scene->frameMan.getFrame(currentFrame->getIndex()+i)->getWidth()&&
                currentFrame->getHeight()==scene->frameMan.getFrame(currentFrame->getIndex()+i)->getHeight()
            ){
                overlaysh.setUniform("ALPHA",((ghostfront-i+1)/(float)ghostfront)*ghostopacity);
                scene->frameMan.getFrame(currentFrame->getIndex()+i)->getSelectedLayer()->bindTexture();
                scene->a.square_vert->draw(GL_TRIANGLES);
            }
        }
    }

    overlaysh.setUniform("ALPHA",1.0f);
    for(int i=0; i<=currentFrame->getSelectedLayerIndex();i++){//layers back and selected layer
        if(currentFrame->getLayer(i)->isVisible()){
            currentFrame->getLayer(i)->bindTexture();
            scene->a.square_vert->draw(GL_TRIANGLES);
        }
    }


    {//active (drawing now)
        glBindTexture(GL_TEXTURE_2D, activeOverlayTex);
        if(activeNeedUpdate){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, activeOverlay->w, activeOverlay->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, activeOverlay->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            activeNeedUpdate=false;
        }

        overlaysh.setUniform("ALPHA", activealpha);
        scene->a.square_vert->draw(GL_TRIANGLES);
    }

    overlaysh.setUniform("ALPHA",1.0f);
    for(int i=currentFrame->getSelectedLayerIndex()+1; i<currentFrame->getLayerCount();i++){//layers front
        if(currentFrame->getLayer(i)->isVisible()){
            currentFrame->getLayer(i)->bindTexture();
            scene->a.square_vert->draw(GL_TRIANGLES);
        }
    }
	
	{//borders
		Shader & border = scene->shMan["overlayborder"];
		border.select().setP(&projection).setM(&model);
		
		bordercoordbuf.bind().attrib(0,2,GL_FLOAT);
		bordercolorbuf.bind().attrib(1,4,GL_FLOAT);
		bordercoordbuf.draw(GL_TRIANGLES);

	}

    if(cameraZoom>15){
        Shader & grid = scene->shMan["overlaygrid"];
        grid.select();
        grid.setP(&projection).setM(&model);
		scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
		scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
		scene->a.square_vert->draw(GL_TRIANGLES);
    }

    if(brushCircleSize>0&&drawX>=0&&drawY>=0&&drawX<currentFrame->getWidth()&&drawY<currentFrame->getHeight())
    {//brush circle
        Shader & shCircle = scene->shMan["brushcircle"];
        glm::mat4 mdl;
        xReset(&mdl);
        xTranslate(&mdl, mouseX, mouseY);
        xScale(&mdl, brushCircleSize * cameraZoomAlpha, brushCircleSize * cameraZoomAlpha);
        xTranslate(&mdl,-0.5,-0.5);
        shCircle.select().setP(&projection).setM(&mdl);
        scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
		scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
		scene->a.square_vert->draw(GL_TRIANGLES);
    }
	
	
	
	if(rotating)
	{//rotation indicator
		Shader & shColor = scene->shGuiColor;
		shColor.select();
		glm::mat4 mdl;
		xReset(&mdl);
		xScale(&mdl, 1.0,1.0);
		shColor.setM(&mdl);
		linebuf.bind();
		float data[]={rotationStart.x*scene->a.getAreaMultipler(),rotationStart.y*scene->a.getAreaMultipler(),(float)mouseX*scene->a.getAreaMultipler(),(float)mouseY*scene->a.getAreaMultipler()};
		linebuf.setData(sizeof(data),data,GL_DYNAMIC_DRAW);
		linebuf.attrib(0,2,GL_FLOAT);
		
		glLineWidth(10);
		shColor.setUniform("COLOR",0.0,0.0,0.0,1.0);
		linebuf.draw(GL_LINES);
		
		glLineWidth(7);
		shColor.setUniform("COLOR",1.0,1.0,1.0,1.0);
		linebuf.draw(GL_LINES);
		
		xReset(&mdl);
		xTranslate(&mdl, rotationStart.x, rotationStart.y);
		xRotate(&mdl, rot);
		xScale(&mdl, 60*currentFrame->getProportions(),60);
		xTranslate(&mdl,-0.5,-0.5);
		overlaysh.select().setUniform("ALPHA",1.0f).setM(&mdl);
		
		scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
		scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
        currentFrame->getSelectedLayer()->bindTexture();
		scene->a.square_vert->draw(GL_TRIANGLES);
		
		scene->shMan["overlayborder"].select().setM(&mdl);
		bordercoordbuf.bind().attrib(0,2,GL_FLOAT);
		bordercolorbuf.bind().attrib(1,4,GL_FLOAT);
		bordercoordbuf.draw(GL_TRIANGLES);

        float rot = SDL_atan2(cameraRot.x,cameraRot.y)*(180.0 / M_PI);
        std::stringstream ss;
        ss<<"Rot: "<<((int)(rot*10))/10.0<<"°";
        textAngle->changeText(ss.str(), scene->a.font24, 0, 0, 0);
        textAngle->setPosition((float)(mouseX)*scene->a.getAreaMultipler(), (float)(mouseY)*scene->a.getAreaMultipler());
        textAngle->setRotation(rot);
        textAngle->render();
	}
}

void Drawer::setBrushCircleSize(int n){
    brushCircleSize = n;
}