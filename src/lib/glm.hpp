#pragma once

#ifdef __linux__
    //#define GLM_FORCE_RADIANS
	#define GLM_ENABLE_EXPERIMENTAL
    #include <glm/glm.hpp>
    #include <glm/gtx/transform.hpp>
#endif

#ifdef __MINGW32__
    //#define GLM_FORCE_RADIANS
	#define GLM_ENABLE_EXPERIMENTAL
    #include <glm/glm.hpp>
    #include <glm/gtx/transform.hpp>
#endif


