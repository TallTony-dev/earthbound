#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "game.hpp"
#include "textures.hpp"
#include <iostream>



int windowWidth = 1000;
int windowHeight = 500;

void UpdateDrawFrame() {
	UpdateGame();
	BeginDrawing();
	ClearBackground(BLACK);
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
	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}

	CloseProgram();
   	return 0;
}