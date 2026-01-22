#include "../raylib/src/raylib.h"
#include "background.hpp"
#include "textures.hpp"
#include <iostream>

#define bgShaderCount 2
Shader bgShaders[bgShaderCount];
int currentBGShaderIndex = 0;

Texture2D rocks;

void InitializeBackground() {

    for (int i = 0; i < bgShaderCount; i++) {
#if defined(PLATFORM_WEB)
		bgShaders[i] = LoadShader(TextFormat("%s%s", GetApplicationDirectory(), "../resources/shaders/es_default.vs"), 
		                          TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/es_background%i.fs", i)));
#else
		bgShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/background%i.fs", i)));
#endif
	}
    rocks = GetTexture("rocks");
}

Vector2 laggardMousePos;

void DrawBackground() {
	float windowHeight = GetScreenHeight();
    float windowWidth = GetScreenWidth();
	BeginShaderMode(bgShaders[currentBGShaderIndex]);
	float totalTime = GetTime();
	Vector2 mousePos = GetMousePosition();
	mousePos.y = -mousePos.y + windowHeight;
	SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "totalTime"), &totalTime, SHADER_UNIFORM_FLOAT);
    if (currentBGShaderIndex == 1) {
	    SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "mousePos"), &mousePos, SHADER_UNIFORM_VEC2);
	    SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "laggardMousePos"), &laggardMousePos, SHADER_UNIFORM_VEC2);
    }
		//std::cout << rocks.id << std::endl;
	laggardMousePos.x += 3 * GetFrameTime() * (mousePos.x - laggardMousePos.x);
	laggardMousePos.y += 3 * GetFrameTime() * (mousePos.y - laggardMousePos.y);
	DrawTexturePro(rocks,{0,0,(float)rocks.width,(float)rocks.height} ,{0,0,windowWidth,windowHeight}, {0, 0}, 0.0f, BLACK);
	EndShaderMode();
}