#include "../game.hpp"
#include "../../raylib/src/raymath.h"
#include "../textures.hpp"
#include "menu.hpp"
#include "hudelements.hpp"
#include "hud.hpp"

void Menu::Activate() {
    isActive_ = true;
}
void Menu::Deactivate() {
    isActive_ = false;
}
bool Menu::IsActive() {
    return isActive_;
}


void Menu::Update(Vector2 mousePos) {
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->Update(mousePos);
    }
}


void Menu::Draw() {
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->Draw();
    }
}

void BattleMenu::SetBattleState(int BattleState) { hudBattleState = BattleState; }

BattleMenu::BattleMenu() {
    elements.push_back(new Button({100,100}, 200, 200, 0, HudAnimBehavior::ExpandContractBounce, HudAnimBehavior::ExpandContractBounce));
    ((Button *)elements[0])->EnableElement();
}

void BattleMenu::Update(Vector2 mousePos) {
    Menu::Update(mousePos);
    //if in a battle
    if (hudBattleState != -1) {
        bool isFirst = false;
        if (prevHudBattleState != hudBattleState)
            isFirst = true;
        if (isFirst) {
            if (hudBattleState == BPLAYERTURN) {
                //enable the choice between attack and item and show most things
                
            } else if (prevHudBattleState == BPLAYERTURN) {
                //disable anything it enables
                
            }

            if (hudBattleState == BITEMSCREEN) {
                //enable item display/inventory
            } else if (prevHudBattleState == BITEMSCREEN) {
                //disable anything it enables
            }

            if (hudBattleState == BATTACKSCREEN) {
                //enable attack buttons
                
            } else if (prevHudBattleState == BATTACKSCREEN) {
                //disable anything it enables
                
            }

            if (hudBattleState == BINACTION) {
                //disable most elements
            } else if (prevHudBattleState == BINACTION) {
                //disable anything it enables
            }
        }
    }
}