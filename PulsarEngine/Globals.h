#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>

#define LIBRARY_PATH "Library/"
#define FOLDERS_PATH "Library/Folders/"
#define MESHES_PATH "Library/Meshes/"
#define MATERIALS_PATH "Library/Materials/"
#define TEXTURES_PATH "Library/Textures/"
#define MODELS_PATH "Library/Models/"
#define ANIMATIONS_PATH "Library/Animations/"
#define BONES_PATH "Library/Bones/"
#define PARTICLES_PATH "Library/ParticleSystems/"
#define SHADERS_PATH "Library/Shaders/"
#define SCENES_PATH "Library/Scenes/"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;
typedef unsigned __int64 uint64;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum Buffers
{
	BUFFIndices,
	BUFFVertices,
	BUFFNormals,
	BUFFTex_coords,
	//BUFFBone_IDs,
	//BUFFBone_weights,
	MAX_BUFFERS, 
};


// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Pulsar Engine"

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }
