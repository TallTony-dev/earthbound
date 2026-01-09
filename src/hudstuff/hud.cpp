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


Menu *GetMenuRef(int i) { return menus[i]; }

void InitializeHud() {
    menus[BATTLEMENU] = new BattleMenu();
    menus[PAUSEMENU] = new Menu();
    menus[MAINMENU] = new Menu();
    menus[BATTLEMENU]->Activate();
}

Vector2 ConvertToHudPos(Vector2 pos) {
    float xscale = GetScreenWidth() / HUDWIDTH;
    float yscale = GetScreenHeight() / HUDHEIGHT;
    pos.x *= xscale;
    pos.y *= yscale;
    return pos;
}

void UpdateHudScale() {
    //scale hud elements and positions here, maybe add aspect ratio member
    /hcghfcycuvu
}

void UpdateHud() {
    for (int i = 0; i < MENUCOUNT; i++) {
        Menu *menu = menus[i];
        if (menu->IsActive()) {
           menu->Update(ConvertToHudPos(GetMousePosition()));
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