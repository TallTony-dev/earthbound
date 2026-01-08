#ifndef hudh
#define hudh 0
#include "../../raylib/src/raylib.h"
#include "menu.hpp"

#define HUDWIDTH 1600.0f
#define HUDHEIGHT 900.0f


#define BATTLEMENU 0
#define PAUSEMENU 1
#define MAINMENU 2

void InitializeHud();
void UpdateHud();
void DrawHud();
BattleMenu *GetBattleMenuRef();

#endif //hudh