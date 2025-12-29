
#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "../raylib/src/rlgl.h"
#include <stdarg.h>
#include "game.hpp"
#include "hud.hpp"
#include "entity.hpp"

void DrawBackground();
void DrawEnemies();

Entity *player = new Entity(100, 10, 10, PATTACKDANCE, PATTACKSPAM);


#define ENEMYSHADERCOUNT 1
Shader enemyShaders[ENEMYSHADERCOUNT];

#define BATTLEENEMIESLENGTH 5
Entity *battleEnemies[BATTLEENEMIESLENGTH];
void RemoveFromBattleEnemies(int index) {battleEnemies[index] = nullptr;}
void RemoveAllFromBattleEnemies() {for(int i = 0; i < BATTLEENEMIESLENGTH; i++) battleEnemies[i] = nullptr;}
int BattleEnemiesCount() { int count; while ( count < 5 && battleEnemies[count] != nullptr) count++; return count;}

void UseAttackOnEnemy(int enemyIndex, int attackNum) {
    battleEnemies[enemyIndex]->GetAttackedBy(attackNum);
}

int mainGameState = INBATTLESTATE; //macros defined in game.h
void SetGameState(int state) {mainGameState = state;}
int prevGameState = -1;

void UpdateGame() {
    if (IsMouseButtonPressed(0))
        CheckClick(GetMousePosition());
    else
        CheckHover(GetMousePosition());
    
    bool isFirst = false;
    if (prevGameState != mainGameState) {
        isFirst = true;
    }

    //now update things/hud based off of gamestate
    if (isFirst) {
        if (mainGameState == MAINMENUSTATE) {
            //ActivateElement(HUD_STARTBTN);
            // ShowElement(HUD_STARTBTN);
        } else if (prevGameState == MAINMENUSTATE) {
            //DeactivateElement(HUD_STARTBTN);
            //HideElement(HUD_STARTBTN);
        }
        if (mainGameState == INBATTLESTATE) {
            EnterBattleState();
        } else if (prevGameState == INBATTLESTATE) {
            ExitBattleState();
        }
    }
    else {
        if (mainGameState == INBATTLESTATE) {

        }
    }
    prevGameState = mainGameState;
}

void DrawGame() {
    DrawBackground();
    DrawHud();
    DrawEnemies();
    if (mainGameState == INBATTLESTATE) {

    }
    //int timeToCross = 4;
    //SetWindowPosition((GetMonitorWidth(0) + windowWidth) * (GetTime() / timeToCross - floor(GetTime() / timeToCross)) - windowWidth, 200 * sin(GetTime() * 4) + GetMonitorHeight(0) / 2 - windowHeight / 2);
}

void DrawEnemies() {
    float xscale = GetScreenWidth() / HUDWIDTH;
    float yscale = GetScreenHeight() / HUDHEIGHT;
    float totalTime = GetTime();

    int activeEnemies = BattleEnemiesCount();

    int xpadding = 200 * xscale;
    int xSpacing = (GetScreenWidth() - xpadding * 2) / activeEnemies;
    float yPos = 0;
    float width = 1000 * xscale;
    float height = 800 * yscale;
    for(int i = 0; i < activeEnemies; i++) {
        float xPos = i * xSpacing + xpadding;
        Entity enemy = *battleEnemies[i];

        float timeSinceDamaged = totalTime - enemy.GetTimeWhenDamaged();
        Vector2 widthheight = {width,height};
        int shaderIndex = enemy.GetShaderIndex();
        if (shaderIndex != EntityShader::None) {
        SetShaderValue(enemyShaders[shaderIndex], GetShaderLocation(enemyShaders[shaderIndex], "totalTime"), &totalTime, SHADER_UNIFORM_FLOAT);
        SetShaderValue(enemyShaders[shaderIndex], GetShaderLocation(enemyShaders[shaderIndex], "timeSinceDamaged"), &timeSinceDamaged, SHADER_UNIFORM_FLOAT);
        BeginShaderMode(enemyShaders[shaderIndex]);
        }

        if (enemy.GetTexture().id != 0)
            DrawTexturePro(enemy.GetTexture(),{0,0,4160,3120} ,{xPos,yPos,xPos + width,xPos + height}, {0, 0}, 0.0f, BLACK);
        else 
            DrawRectangleV({xPos, yPos}, {width, height}, BLACK);

        if (shaderIndex != EntityShader::None)
            EndShaderMode();
    }
}

#define bgShaderCount 2
Shader bgShaders[bgShaderCount];
int currentBGShaderIndex = 0;

Texture2D testTex;
Vector2 laggardMousePos;

void InitializeGame() {
    testTex = LoadTexture(TextFormat("%s%s", GetApplicationDirectory(), "../resources/textures/testrocks.png"));
    Texture2D texture = { rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };

    SetShapesTexture(texture, {0.0f, 0.0f, 1.0f, 1.0f });
    InitializeHud();

    battleEnemies[0] = new Entity(100, 10, 10, EATTACKJAB, EATTACKSPIN);

	for (int i = 0; i < bgShaderCount; i++)
		bgShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/background%i.fs", i)));
    for (int i = 0; i < ENEMYSHADERCOUNT; i++)
 		enemyShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/enemy%i.fs", i)));
       
}

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
	laggardMousePos.x += 3 * GetFrameTime() * (mousePos.x - laggardMousePos.x);
	laggardMousePos.y += 3 * GetFrameTime() * (mousePos.y - laggardMousePos.y);
	DrawTexturePro(testTex,{0,0,4160,3120} ,{0,0,windowWidth,windowHeight}, {0, 0}, 0.0f, BLACK);
	EndShaderMode();
}