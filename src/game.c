#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#include "../raylib/src/rlgl.h"
#include <stdarg.h>
#include "game.h"
#include "hud.h"

void DrawBackground();
void DrawEnemies();

Entity player;


Shader enemyShaders[ENEMYINDEXCOUNT];
Texture2D enemyTextures[ENEMYINDEXCOUNT];

#define BATTLEENEMIESLENGTH 5
Entity battleEnemies[BATTLEENEMIESLENGTH];
void RemoveFromBattleEnemies(int index) {battleEnemies[index].typeIndex = -2;}
void RemoveAllFromBattleEnemies() {for(int i = 0; i < BATTLEENEMIESLENGTH; i++) battleEnemies[i].typeIndex = -2;}
int BattleEnemiesCount() { int count; while ( count < 5 && battleEnemies[count].typeIndex != -2) count++; return count;}


int gameState = INBATTLESTATE; //macros defined in game.h
void SetGameState(int state) {gameState = state;}
int prevGameState = -1;

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
        if (gameState == MAINMENUSTATE) {
            ActivateElement(HUD_STARTBTN);
            ShowElement(HUD_STARTBTN);
        } else if (prevGameState == MAINMENUSTATE) {
            DeactivateElement(HUD_STARTBTN);
            HideElement(HUD_STARTBTN);
        }
        if (gameState == INBATTLESTATE) {
            ActivateElement(HUD_ATTACKBTN);
            ActivateElement(HUD_HEALTHBAR);
            ActivateElement(HUD_SNOOZEBAR);
            ActivateElement(HUD_ITEMBTN);
        }
    }
    else {
        if (gameState == INBATTLESTATE) {
            
        }
    }
    prevGameState = gameState;
}

void DrawGame() {
    DrawBackground();
    DrawHud();
    DrawEnemies();
    if (gameState == INBATTLESTATE) {

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
    int yPos = 0;
    int width = 1000 * xscale;
    int height = 800 * yscale;
    for(int i = 0; i < activeEnemies; i++) {
        int xPos = i * xSpacing + xpadding;
        Entity enemy = battleEnemies[i];


        Vector2 widthheight = (Vector2){width,height};
        SetShaderValue(enemyShaders[enemy.typeIndex], GetShaderLocation(enemyShaders[enemy.typeIndex], "totalTime"), &totalTime, SHADER_UNIFORM_FLOAT);

        BeginShaderMode(enemyShaders[enemy.typeIndex]);
        
        if (enemyTextures[enemy.typeIndex].id != 0)
            DrawTexturePro(enemyTextures[enemy.typeIndex],(struct Rectangle) {0,0,4160,3120} ,(struct Rectangle) {xPos,yPos,xPos + width,xPos + height}, (struct Vector2) {0, 0}, 0.0f, BLACK);
        else 
            DrawRectangleV((Vector2){xPos, yPos},(Vector2){width, height}, BLACK);
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

    SetShapesTexture(texture, (Rectangle){ 0.0f, 0.0f, 1.0f, 1.0f });
    for (int i = 0; i < BATTLEENEMIESLENGTH; i++) 
        battleEnemies[i].typeIndex = -2;
    InitializeEntity(&player, 100, 10, 10, PLAYERTYPE, PATTACKDANCE, PATTACKSPAM, -1, -1);
    InitializeHud();

    InitializeEntity(battleEnemies, 100, 10, 10, ENEMYGLEEBTYPE, EATTACKJAB, EATTACKSPIN, -1, -1);

	for (int i = 0; i < bgShaderCount; i++)
		bgShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/background%i.fs", i)));
    for (int i = 0; i < ENEMYINDEXCOUNT; i++)
 		enemyShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/enemy%i.fs", i)));
       
}

void DrawBackground() {
	int windowHeight = GetScreenHeight();
    int windowWidth = GetScreenWidth();
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

void InitializeEntity(Entity *entity, float health, float attackPow, float defence, int type, int attack1, int attack2, int attack3, int attack4) {
    entity->health = health;
    entity->maxHealth = health;
    entity->attackPower = attackPow;
    entity->defence = defence;
    entity->typeIndex = type;
    entity->attack1 = attack1;
    entity->attack2 = attack2;
    entity->attack3 = attack3;
    entity->attack4 = attack4;
}