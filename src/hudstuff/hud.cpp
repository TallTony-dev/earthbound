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

void UpdateHud() {
    Vector2 mousePos = GetMousePosition();
    float xscale = HUDWIDTH / GetScreenWidth();
    float yscale = HUDHEIGHT / GetScreenHeight();
    mousePos.x *= xscale;
    mousePos.y *= yscale;
    for (int i = 0; i < MENUCOUNT; i++) {
        Menu *menu = menus[i];
        if (menu->IsActive()) {
           menu->Update(mousePos);
        }
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