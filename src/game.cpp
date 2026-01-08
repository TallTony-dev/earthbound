#include <string>
#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "../raylib/src/rlgl.h"
#include <stdarg.h>
#include "game.hpp"
#include "hudstuff/hud.hpp"
#include "entity.hpp"
#include "background.hpp"
#include "textures.hpp"
#include <iostream>

void DrawBackground();
void DrawEnemies();

Entity *player = nullptr;


#define ENEMYSHADERCOUNT 1
Shader enemyShaders[ENEMYSHADERCOUNT];

#define BATTLEENEMIESLENGTH 5
Entity *battleEnemies[BATTLEENEMIESLENGTH];
void RemoveFromBattleEnemies(int index) {battleEnemies[index] = nullptr;} //DOESNT DEALLOC MEMORY
void RemoveAllFromBattleEnemies() {for(int i = 0; i < BATTLEENEMIESLENGTH; i++) battleEnemies[i] = nullptr;}
int BattleEnemiesCount() { int count; while ( count < 5 && battleEnemies[count] != nullptr) count++; return count;}

void UseAttackOnEnemy(int enemyIndex, int attackNum) {
    battleEnemies[enemyIndex]->GetAttackedBy(attackNum);
}

int mainGameState = INBATTLESTATE; //macros defined in game.h
void SetGameState(int state) {mainGameState = state;}
int prevGameState = -1;

void UpdateGame() {
    UpdateHud();
    
    
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
            //EnterBattleState();
        } else if (prevGameState == INBATTLESTATE) {
            //ExitBattleState();
        }
    }
    else {
        if (mainGameState == INBATTLESTATE) {

        }
    }
    prevGameState = mainGameState;
}


void DrawFramerate(int size) {
    int framerate = 1 / GetFrameTime();
    std::string framerateText = std::to_string(framerate);
    DrawText(framerateText.c_str(), 0, 0, size, GREEN);
}

void DrawGame() {
    DrawBackground();
    //std::cout << "Drew background" << std::endl;
    DrawHud();
    //std::cout << "Drew HUD" << std::endl;
    DrawEnemies();
    //std::cout << "Drew enemies" << std::endl;
    DrawFramerate(40);
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
    int xSpacing = (GetScreenWidth() - xpadding * 2) / (activeEnemies == 0 ? 1 : activeEnemies);
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
            DrawTexturePro(enemy.GetTexture(),{0,0,4160,3120} ,{xPos,yPos,xPos + width,yPos + height}, {0, 0}, 0.0f, BLACK);
        else 
            DrawRectangleV({xPos, yPos}, {width, height}, BLACK);

        if (shaderIndex != EntityShader::None)
            EndShaderMode();
    }
}





void InitializeGame() {
    Texture2D texture = { rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    SetWindowTitle("Waow");
    SetShapesTexture(texture, {0.0f, 0.0f, 1.0f, 1.0f });
    InitializeTextures(); //must be called first
    InitializeHud();
    InitializeBackground();
    SetTargetFPS(200);

    player = new Entity(100, 10, 10, PATTACKDANCE, PATTACKSPAM);
    battleEnemies[0] = new Entity(100, 10, 10, EATTACKJAB, EATTACKSPIN);

	for (int i = 0; i < ENEMYSHADERCOUNT; i++)
 		enemyShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/enemy%i.fs", i)));
       
}

