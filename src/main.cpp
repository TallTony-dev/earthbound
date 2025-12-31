#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "game.hpp"
#include "textures.hpp"
#include <iostream>
// #if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
// #else   // PLATFORM_ANDROID, PLATFORM_WEB
//     #define GLSL_VERSION            100
// #endif
//if you want cross platform, use this to load shaders based off version


int windowWidth = 1000;
int windowHeight = 500;

void Update() {
	//use GetFrameTime() for deltaTime
	UpdateGame();
}

void Initialize() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(windowWidth, windowHeight, "Template-4.0.0");
	InitializeGame();
}

void CloseProgram() {
	UnloadTextures();
   	CloseWindow();
}


int main()
{
	std::cout << "Running main" << std::endl;
	Initialize();

	while (!WindowShouldClose()) {
		Update();
		//std::cout << "Updated" << std::endl;
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawGame();
		//std::cout << "Drew" << std::endl;
		EndDrawing();
	}	
	CloseProgram();
   	return 0;
}