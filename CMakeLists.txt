#WARNING
#I'm not the CMake expert. 
#You can help me if you want.

cmake_minimum_required(VERSION 3.0)
project(MPS)

set(TYPE "Release")#Debug, Normal, Release
set(PLATFORM "Linux")#Linux, Windows
set(ROOT "./src/") 

add_definitions(-DInstall=${Install})

if(TYPE STREQUAL "Debug")
	set (CMAKE_CXX_FLAGS "-Wall -I ./src -g -std=c++11")
endif()
if(TYPE STREQUAL "Normal")
	set (CMAKE_CXX_FLAGS "-Wall -I ./src -std=c++11")
endif()
if(TYPE STREQUAL "Release")
	set (CMAKE_CXX_FLAGS "-Wall -I ./src -O3 -std=c++11")
endif()

if(PLATFORM STREQUAL "Linux")
	set( CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" )
	set( CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" )
	#set( CMAKE_C_COMPILER /usr/bin/zapcc)
	#set( CMAKE_CXX_COMPILER /usr/bin/zapcc++ )
	set( CMAKE_C_COMPILER gcc )
	set( CMAKE_CXX_COMPILER g++ )
endif()
if(PLATFORM STREQUAL "Windows") 
	set( CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-static-libgcc -static-libstdc++" )
	set( CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static-libgcc -static-libstdc++" )
	set( CMAKE_C_COMPILER x86_64-w64-mingw32-gcc )
	set( CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++ )
endif()


SET(PROGRAM_SRCS
	${ROOT}/app.cpp
	${ROOT}/app_run.cpp
	${ROOT}/error.cpp
	${ROOT}/framebuffermanager.cpp
	${ROOT}/main.cpp
	${ROOT}/pixel.cpp
	${ROOT}/shaderload.cpp
	${ROOT}/shadermanager.cpp
	${ROOT}/texload.cpp
	${ROOT}/texturemanager.cpp
	${ROOT}/timestep.cpp
	${ROOT}/transform.cpp
	${ROOT}/fps.cpp
	${ROOT}/lib/SDL2_rotozoom.cpp
	${ROOT}/configmanager.cpp
	${ROOT}/discord.cpp
	
	${ROOT}/render/buffer.cpp
	${ROOT}/render/func.cpp

	${ROOT}/scenes/editor/drawer.cpp
	${ROOT}/scenes/editor/editor.cpp
	${ROOT}/scenes/editor/frame.cpp 
	${ROOT}/scenes/editor/framemanager.cpp
	${ROOT}/scenes/editor/history.cpp
	${ROOT}/scenes/editor/thememanager.cpp
	${ROOT}/scenes/editor/layermanager.cpp
	${ROOT}/scenes/editor/actionlog.cpp
	${ROOT}/scenes/editor/colorselector/colorselector.cpp
	${ROOT}/scenes/editor/frameselector/frameselector.cpp
	
	${ROOT}/scenes/editor/menu/menu.cpp
	${ROOT}/scenes/editor/menu/menubutton.cpp
	${ROOT}/scenes/editor/menu/tinyfiledialogs.cpp

	${ROOT}/scenes/editor/menu/export/gif.cpp
	
	${ROOT}/scenes/editor/gui/button.cpp
	${ROOT}/scenes/editor/gui/checkbox.cpp
	${ROOT}/scenes/editor/gui/text.cpp
	${ROOT}/scenes/editor/gui/valuebox.cpp
	
	${ROOT}/scenes/editor/toolbox/toolbox.cpp
	${ROOT}/scenes/editor/toolbox/tool/floodfill.cpp
	${ROOT}/scenes/editor/toolbox/tool/line.cpp
	${ROOT}/scenes/editor/toolbox/tool/pencil.cpp
	${ROOT}/scenes/editor/toolbox/tool/pipette.cpp
	${ROOT}/scenes/editor/toolbox/tool/spray.cpp
	${ROOT}/scenes/editor/toolbox/tool/tool.cpp
	${ROOT}/scenes/editor/toolbox/tool/eraser.cpp
	${ROOT}/scenes/editor/toolbox/tool/rectangle.cpp
	)
	 
if(PLATFORM STREQUAL "Linux")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/APP/linux64/)
endif()
if(PLATFORM STREQUAL "Windows")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/APP/win64/)
endif()


add_executable(MPS "${PROGRAM_SRCS}")


if(PLATFORM STREQUAL "Linux")
	set_target_properties(MPS PROPERTIES OUTPUT_NAME "MPS64")

	target_link_libraries (MPS -lSDL2main)
	target_link_libraries (MPS -lSDL2)
	target_link_libraries (MPS -lSDL2_image)
	target_link_libraries (MPS -lSDL2_ttf)
	target_link_libraries (MPS -lGLEW)
	target_link_libraries (MPS -lGL)
	target_link_libraries (MPS -lpthread)
	target_link_libraries (MPS -no-pie)
	target_link_libraries (MPS ${CMAKE_SOURCE_DIR}/src/lib/libdiscord-rpc.a)
endif() 

if(PLATFORM STREQUAL "Windows")
	set_target_properties(MPS PROPERTIES OUTPUT_NAME "MPS64.exe")
	
	target_link_libraries (MPS -lmingw32)
	target_link_libraries (MPS -lSDL2main)
	target_link_libraries (MPS -lSDL2)
	target_link_libraries (MPS -lSDL2_image)
	target_link_libraries (MPS -lSDL2_ttf)
	target_link_libraries (MPS -lglew32s)
	target_link_libraries (MPS -lopengl32)
	target_link_libraries (MPS -lole32)
	target_link_libraries (MPS -loleaut32)
	target_link_libraries (MPS -lcomdlg32)
	if(TYPE STREQUAL "Release")
		target_link_libraries (MPS -mwindows)
	endif()
endif()

