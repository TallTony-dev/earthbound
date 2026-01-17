#include "menu.hpp"
#include "hud.hpp"


#define MENUCOUNT 3
Menu *menus[MENUCOUNT];

// void SetBattleStatePlayerTurn() {hudBattleState = BPLAYERTURN; }
// void SetBattleStateItemScreen() {hudBattleState = BITEMSCREEN; }
// void SetBattleStateAttackScreen() {hudBattleState = BATTACKSCREEN; }
// void SetBattleStateInAction() {hudBattleState = BINACTION; }
// void EnterBattleState() { hudBattleState = BPLAYERTURN; }
// void ExitBattleState() { hudBattleState = BOUTOFBATTLE; }

float hudScale = 1;

Menu *GetMenuRef(int i) { return menus[i]; }

void InitializeHud() {
    menus[BATTLEMENU] = new BattleMenu();
    menus[PAUSEMENU] = new Menu();
    menus[MAINMENU] = new Menu();
    menus[BATTLEMENU]->Activate();
}

Vector2 ConvertToHudPos(Vector2 screenPos) {
    float xscale = GetScreenWidth() / HUDWIDTH;
    float yscale = GetScreenHeight() / HUDHEIGHT;
    screenPos.x *= xscale;
    screenPos.y *= yscale;
    return screenPos;
}
Vector2 ConvertToScreenPos(Vector2 hudPos) {
    float xscale = HUDWIDTH / GetScreenWidth();
    float yscale = HUDHEIGHT / GetScreenHeight();
    hudPos.x *= xscale;
    hudPos.y *= yscale;
    return hudPos;
}

void UpdateHudScale() {
    //scale hud elements and positions here
    //use old scale to find new positions and scales
    //TODO: implement this pls ty

}

void UpdateHud() {
    for (int i = 0; i < MENUCOUNT; i++) {
        Menu *menu = menus[i];
        if (menu->IsActive()) {
           menu->Update(GetMousePosition()); //may have to convert mousepos to hudpos when scaling is implemented properly
        }
    }
    if (IsWindowResized()) {
        UpdateHudScale();
    }
    UpdateHudAnims();

}

void DrawHud() {
    float xscale = GetScreenWidth() / HUDWIDTH;
    float yscale = GetScreenHeight() / HUDHEIGHT;

    for (int i = 0; i < MENUCOUNT; i++) {
        if (menus[i]->IsActive()) {
            menus[i]->Draw();
        }
    }
}