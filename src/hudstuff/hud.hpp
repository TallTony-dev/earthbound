#pragma once
#include "../../raylib/src/raylib.h"
#include "menu.hpp"

#define HUDWIDTH 1600.0f
#define HUDHEIGHT 900.0f
Vector2 ConvertToHudPos(Vector2 screenPos);
Vector2 ConvertToScreenPos(Vector2 hudPos);

extern float hudScale; //modifies height and width of elements

#define BATTLEMENU 0
#define PAUSEMENU 1
#define MAINMENU 2
Menu *GetMenuRef(int i);

void InitializeHud();
void UpdateHud();
void DrawHud();
BattleMenu *GetBattleMenuRef();