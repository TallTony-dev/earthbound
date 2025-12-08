#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "game.h"
#include "hud.h"

void DrawBackground(int windowHeight, int windowWidth);


int gameState = 0; //macros defined in game.h
void SetGameState(int state) {gameState = state;}

int prevGameState;

void UpdateGame() {
    if (IsMouseButtonPressed(0))
        CheckClick(GetMousePosition());
    else
        CheckHover(GetMousePosition());
    
    bool isFirst = false;
    if (prevGameState != gameState) {
        isFirst = true;
    }

    //now update things/hud based off of gamestate
    if (isFirst) {
        if (gameState == MAINMENU) {
            ActivateElement(HUD_STARTBTN);
            ShowElement(HUD_STARTBTN);
            
        } else if (prevGameState == MAINMENU) {
            DeactivateElement(HUD_STARTBTN);
            HideElement(HUD_STARTBTN);
        }
    }
    else {
        
    }
    prevGameState = gameState;
}

void DrawGame(int windowHeight, int windowWidth) {
    DrawBackground(windowHeight, windowWidth);
    DrawHud(windowHeight, windowWidth);
    //int timeToCross = 4;
    //SetWindowPosition((GetMonitorWidth(0) + windowWidth) * (GetTime() / timeToCross - floor(GetTime() / timeToCross)) - windowWidth, 200 * sin(GetTime() * 4) + GetMonitorHeight(0) / 2 - windowHeight / 2);
}


#define bgShaderCount 2
Shader bgShaders[bgShaderCount];
int currentBGShaderIndex = 0;

Texture2D testTex;
Vector2 laggardMousePos;

void InitializeGame() {
    testTex = LoadTexture("../resources/textures/testrocks.png");
    InitializeHud();
	for (int i = 0; i < bgShaderCount; i++)
		bgShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/background%i.fs", i)));
}

void DrawBackground(int windowHeight, int windowWidth) {
	
	BeginShaderMode(bgShaders[currentBGShaderIndex]);
	float totalTime = GetTime();
	Vector2 mousePos = GetMousePosition();
	mousePos.y = -mousePos.y + windowHeight;
	SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "totalTime"), &totalTime, SHADER_UNIFORM_FLOAT);
    if (currentBGShaderIndex == 1) {
	    SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "mousePos"), &mousePos, SHADER_UNIFORM_VEC2);
	    SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "laggardMousePos"), &laggardMousePos, SHADER_UNIFORM_VEC2);
    }
	laggardMousePos.x += 3 * GetFrameTime() * (mousePos.x - laggardMousePos.x);
	laggardMousePos.y += 3 * GetFrameTime() * (mousePos.y - laggardMousePos.y);
	DrawTexturePro(testTex,(struct Rectangle) {0,0,4160,3120} ,(struct Rectangle) {0,0,windowWidth,windowHeight}, (struct Vector2) {0, 0}, 0.0f, BLACK);
	EndShaderMode();
}
