#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "game.hpp"
#include "textures.hpp"
#include <iostream>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


int windowWidth = 1000;
int windowHeight = 500;

void UpdateDrawFrame() {
	UpdateGame();
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawGame();
	EndDrawing();
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

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
#endif

	CloseProgram();
   	return 0;
}