#include "colorselector.hpp"
#include "pixel.hpp"
#include "shadermanager.hpp"
#include "../editor.hpp"
#include "transform.hpp"
#include <sstream>
#include <iomanip>

#include "render/func.hpp"


typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

void colorSelector::setColor(Uint8 r, Uint8 g, Uint8 b){
    RgbColor rgb = {r,g,b};
    HsvColor hsv = RgbToHsv(rgb);
    hue=hsv.h;
    saturation=hsv.s;
    value=hsv.v;
    updateRGB();
    updateColorsTexture();
    updateText();
}

void colorSelector::updateText(){
    stringstream ss;
    ss<<"#";
    ss<<setfill('0') << setw(2) << std::hex << red;
    ss<<setfill('0') << setw(2) << std::hex << green;
    ss<<setfill('0') << setw(2) << std::hex << blue;
    ss<<std::dec<<" "<<red<<","<<green<<","<<blue;
    if(value>128){
        text_info.changeText(ss.str(),scene->a.font16,0,0,0);
    }
    else{
        text_info.changeText(ss.str(),scene->a.font16,255,255,255);
    }
}

void colorSelector::init(sceneEditor * scene){
    this->scene = scene;
    SDL_Surface * verticalBar = SDL_CreateRGBSurface(0, 32, 256, 32, 0x000000FF , 0x0000FF00, 0x00FF0000, 0xFF000000);
    RgbColor rgb;
    HsvColor hsv;
    hsv.s=255;
    hsv.v=255;
    for(int i=0;i<256;i++){
        hsv.h=i;
        rgb = HsvToRgb(hsv);
        for(int j=0;j<32;j++){
            putpixel(verticalBar, j, i, SDL_MapRGB(verticalBar->format, rgb.r, rgb.g, rgb.b));
        }
    }
    verticalbartexture.load(verticalBar, TEXSPACE_RGB, TEXFILTERING_LINEAR);
    SDL_FreeSurface(verticalBar);

    colorstexture.create(TEXFILTERING_LINEAR);
    updateColorsTexture();

    text_info.init(scene);
    text_info.setXAlign(0).setYAlign(1);

    updateRGB();
    updateText();
}

void colorSelector::updateColorsTexture(){
    RgbColor rgb;
    HsvColor hsv;

    hsv.h=hue;

    SDL_Surface * colorssurface = SDL_CreateRGBSurface(0, 256, 256, 32, 0x000000FF , 0x0000FF00, 0x00FF0000, 0xFF000000);
    for(int y=0;y<256;y++){
        for(int x=0;x<256;x++){
            hsv.s=x;
            hsv.v=255-y;

            rgb = HsvToRgb(hsv);

            putpixel(colorssurface,x,y,SDL_MapRGB(colorssurface->format, rgb.r,rgb.g,rgb.b));
        }
    }
    colorstexture.update(colorssurface);
    SDL_FreeSurface(colorssurface);
}

colorSelector::~colorSelector(){

}

void colorSelector::render(){
    Shader & sh = scene->shGui;
    sh.select();

    int width = scene->a.getAreaWidth();

    colorsX=width-selectorheight-25;

    Texture & scrolltexture = scene->thMan["cs/scroll"];
    Texture & pointtexture = scene->thMan["cs/point"];

    scene->a.square_vert->bind().attrib(0,2,GL_FLOAT);
	scene->a.square_uv->bind().attrib(1,2,GL_FLOAT);
	

    {//hue bar
        xReset(&model);
        xTranslate(&model, width-16, 0);
        xScale(&model,16, selectorheight);
        verticalbartexture.select();
        sh.setM(&model);
        scene->a.square_vert->draw(GL_TRIANGLES);
    }
    {//hue colors
        if(updatecolors){
            updateColorsTexture();
            updatecolors=false;
        }
        xReset(&model);
        xTranslate(&model, colorsX, 0);
        xScale(&model,selectorheight, selectorheight);
        colorstexture.select();
        sh.setM(&model);
        scene->a.square_vert->draw(GL_TRIANGLES);
    }

    {//hue bar scroll
        xReset(&model);
        xTranslate(&model, width-scrolltexture.getWidth(), (hue/255.0)*selectorheight-scrolltexture.getHeight()/2);
        xScale(&model,scrolltexture.getWidth(), scrolltexture.getHeight());
        scrolltexture.select();
        sh.setM(&model);

        scene->a.square_vert->draw(GL_TRIANGLES);
    }
    {//hue colors point
        xReset(&model);
        xTranslate(&model, colorsX + (saturation/255.0)*selectorheight - 10, (1.0-value/255.0)*selectorheight - 10);
        xScale(&model,20, 20);
        pointtexture.select();
        sh.setM(&model);

        scene->a.square_vert->draw(GL_TRIANGLES);
    }

    {//current color
        xReset(&model);
        xTranslate(&model, colorsX, selectorheight+8);
        xScale(&model, scene->a.getAreaWidth()-colorsX,32);
        scene->shGuiColor.select().setM(&model).setUniform("COLOR",red/255.0,green/255.0,blue/255.0,1.0);
        scene->a.square_vert->draw(GL_TRIANGLES);

        text_info.setPosition(colorsX+8,selectorheight+8+16);
        text_info.render();
    }
}

void colorSelector::changeHueToCursorPosition(int y){
    int resulthue=(y/(float)selectorheight)*255.0;
    if(resulthue<0)resulthue=0;
    if(resulthue>255)resulthue=255;
    hue=resulthue;
    updatecolors=true;

    updateRGB();
    updateText();
}

void colorSelector::changeColorToCursorPosition(int x, int y){
    int normalizedX = (x-colorsX)/(float)selectorheight * 255;
    int normalizedY = y/(float)selectorheight * 255;

    saturation = normalizedX;
    value = 255-normalizedY;

    if(saturation<0)saturation=0;
    else if(saturation>255)saturation=255;

    if(value<0)value=0;
    else if(value>255)value=255;

    updateRGB();
    updateText();
}
void colorSelector::updateRGB(){
    HsvColor hsv;
    hsv.h=hue;
    hsv.s=saturation;
    hsv.v=value;
    RgbColor rgb = HsvToRgb(hsv);
    red=rgb.r;
    green=rgb.g;
    blue=rgb.b;

    scene->drawer.setColor(red,green,blue);

}

bool colorSelector::pushEvent(SDL_Event * evt){
    used=false;

    switch(evt->type){
        case SDL_MOUSEBUTTONDOWN:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                if(evt->button.x*scene->a.getAreaMultiplier()>=scene->a.getAreaWidth()-selectorheight-25-5&&evt->button.y*scene->a.getAreaMultiplier()<selectorheight+5){
                    down=true;

                    if(evt->button.x*scene->a.getAreaMultiplier()>=scene->a.getAreaWidth()-16&&evt->button.y*scene->a.getAreaMultiplier()<selectorheight){
                        ischanginghue=true;
                        changeHueToCursorPosition(evt->button.y*scene->a.getAreaMultiplier());
                    }
                    else{
                        ischanginghue=false;
                    }

                    if(evt->button.x*scene->a.getAreaMultiplier()>=colorsX&&evt->button.x*scene->a.getAreaMultiplier()<colorsX+selectorheight&&evt->button.y*scene->a.getAreaMultiplier()<selectorheight){
                        ischangingcolor=true;
                        changeColorToCursorPosition(evt->button.x*scene->a.getAreaMultiplier(), evt->button.y*scene->a.getAreaMultiplier());
                    }
                    else{
                        ischangingcolor=false;
                    }

                    used=true;
                }
            }
            break;
        }
        case SDL_MOUSEMOTION:{
            if(down){
                if(ischanginghue){
                    changeHueToCursorPosition(evt->motion.y*scene->a.getAreaMultiplier());
                }
                if(ischangingcolor){
                    changeColorToCursorPosition(evt->motion.x*scene->a.getAreaMultiplier(), evt->motion.y*scene->a.getAreaMultiplier());
                }

                used=true;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            if(evt->button.button==SDL_BUTTON_LEFT){
                down=false;
            }
            break;
        }
    }

    return used;
}

bool colorSelector::pushGlobalEvent(GlobalEvent evt){
    bool used=false;

    switch(evt){
        case GlobalEvent::GuiChange:{
            updateText();
            break;
        }
        default:break;
    }

    return used;
}