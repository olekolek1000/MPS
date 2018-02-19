#pragma once

#include <string>

#ifdef __ANDROID__
    #define LOC_ROOT ""
#else
    #define LOC_ROOT "../assets/"
#endif
 
//#define FASTSTART 
//#define USE_FRAMEBUFFER
