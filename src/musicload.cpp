#include "musicload.h"
#include "defines.h"

Mix_Music * musicload(std::string location){
	return Mix_LoadMUS(std::string(std::string(LOC_ROOT)+location).c_str());
}