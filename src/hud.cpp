#include "hud.hpp"
#include "game.hpp"
#include "../raylib/src/raymath.h"
#include "textures.hpp"

HudElement elements[HUDELEMENTCOUNT];

/// @brief enables the whole element, both shows and activates 
void EnableElement(int element) {
    elements[element].isActive = true;
    elements[element].isHidden = false;
}
/// @brief counterpart to enableelement, hides and deactivates
void DisableElement(int element) {
    elements[element].isActive = false;
    elements[element].isHidden = true;
}
void ActivateElement(int element) {
    elements[element].isActive = true;
}
void DeactivateElement(int element) {
    elements[element].isActive = false;
}
void ToggleElementActive(int element) {
    elements[element].isActive = !elements[element].isActive;
}
void ShowElement(int element) {
    elements[element].isHidden = false;
}
void HideElement(int element) {
    elements[element].isHidden = true;
}
void ToggleElementVis(int element) {
    elements[element].isHidden = !elements[element].isHidden;
}


#define BOUTOFBATTLE -1
#define BPLAYERTURN 0
#define BITEMSCREEN 1
#define BATTACKSCREEN 2
#define BINACTION 3
int hudBattleState = BOUTOFBATTLE;
int prevHudBattleState = BOUTOFBATTLE;

void SetBattleStatePlayerTurn() {hudBattleState = BPLAYERTURN; }
void SetBattleStateItemScreen() {hudBattleState = BITEMSCREEN; }
void SetBattleStateAttackScreen() {hudBattleState = BATTACKSCREEN; }
void SetBattleStateInAction() {hudBattleState = BINACTION; }

void EnterBattleState() {
    hudBattleState = BPLAYERTURN;
}

void ExitBattleState() {
    hudBattleState = BOUTOFBATTLE;
}

void InitializeHud() {

    HudElement elements_[HUDELEMENTCOUNT] = { 
        #include "hudembed.txt" 
    };
    for (int i = 0; i < HUDELEMENTCOUNT; i++) {
        elements[i] = elements_[i];
        elements[i].elementIndex = i;
    }
    elements[HUD_BATTACK1BTN].texture = GetTexture("rocks");
}

void UpdateHud() {
    //manage battlestate
    bool isFirst = false;
    if (prevHudBattleState != hudBattleState)
        isFirst = true;
    if (isFirst) {
        if (hudBattleState == BPLAYERTURN) {
            //enable the choice between attack and item and show most things
            EnableElement(HUD_BATTACKBTN);
            EnableElement(HUD_BHEALTHBAR);
            EnableElement(HUD_BSNOOZEBAR);
            EnableElement(HUD_BITEMBTN);
        } else if (prevHudBattleState == BPLAYERTURN) {
            //disable anything it enables
            DisableElement(HUD_BATTACKBTN);
            DisableElement(HUD_BHEALTHBAR);
            DisableElement(HUD_BSNOOZEBAR);
            DisableElement(HUD_BITEMBTN);
        }

        if (hudBattleState == BITEMSCREEN) {
            //enable item display/inventory
        } else if (prevHudBattleState == BITEMSCREEN) {
            //disable anything it enables
        }

        if (hudBattleState == BATTACKSCREEN) {
            //enable attack buttons
            EnableElement(HUD_BATTACK1BTN);
            EnableElement(HUD_BATTACK2BTN);
            EnableElement(HUD_BATTACK3BTN);
            EnableElement(HUD_BATTACK4BTN);
        } else if (prevHudBattleState == BATTACKSCREEN) {
            //disable anything it enables
            DisableElement(HUD_BATTACK1BTN);
            DisableElement(HUD_BATTACK2BTN);
            DisableElement(HUD_BATTACK3BTN);
            DisableElement(HUD_BATTACK4BTN);
        }

        if (hudBattleState == BINACTION) {
            //disable most elements
        } else if (prevHudBattleState == BINACTION) {
            //disable anything it enables
        }
    }
}

void DrawHud() {
    float xscale = GetScreenWidth() / HUDWIDTH;
    float yscale = GetScreenHeight() / HUDHEIGHT;

    for (int i = 0; i < HUDELEMENTCOUNT; i++) {
        if (!elements[i].isHidden) {
            if (elements[i].texture.id != 0)
                DrawTexturePro(elements[i].texture, {0,0,(float)elements[i].texture.width,(float)elements[i].texture.height}, {elements[i].pos.x * xscale, elements[i].pos.y * yscale, elements[i].width * xscale, elements[i].height * yscale}, {}, elements[i].rotation, WHITE);
            else
                DrawRectangleV({elements[i].pos.x * xscale, elements[i].pos.y * yscale}
                ,{elements[i].width * xscale, elements[i].height * yscale}, DARKBLUE);
            //SetWindowPosition((GetMonitorWidth(0) + 1000) * (GetTime() / 4 - floor(GetTime() / 4)) - 1000, 200 * sin(GetTime() * 4) + GetMonitorHeight(0) / 2 - 600 / 2);
        }
    }
}

void CheckClick(Vector2 mousePos) {
    float xscale = HUDWIDTH / GetScreenWidth();
    float yscale = HUDHEIGHT / GetScreenHeight();
    mousePos.x *= xscale;
    mousePos.y *= yscale;
    for (int i = 0; i < HUDELEMENTCOUNT; i++) {
        if (elements[i].isActive && elements[i].onClickFunction != 0) {
            if (elements[i].rotation == 0) {
                if (mousePos.x > elements[i].pos.x && mousePos.x < elements[i].pos.x + elements[i].width && mousePos.y > elements[i].pos.y && mousePos.y < elements[i].pos.y + elements[i].height) {
                    elements[i].onClickFunction();
                }
            }
        }
    }
}

void CheckHover(Vector2 mousePos) {
    float xscale = HUDWIDTH / GetScreenWidth();
    float yscale = HUDHEIGHT / GetScreenHeight();
    mousePos.x *= xscale;
    mousePos.y *= yscale;
    for (int i = 0; i < HUDELEMENTCOUNT; i++) {
        if (elements[i].isActive && elements[i].onHoverFunction != 0) {
            if (elements[i].rotation == 0) {
                bool intersects = false;
                if ( mousePos.x > elements[i].pos.x && mousePos.x < elements[i].pos.x + elements[i].width && mousePos.y > elements[i].pos.y && mousePos.y < elements[i].pos.y + elements[i].height)
                    intersects = true;
                if (intersects && !elements[i].wasHovered) {
                    elements[i].onHoverFunction();
                    elements[i].wasHovered = true;
                }
                else if (!intersects && elements[i].wasHovered) {
                    elements[i].wasHovered = false;
                    elements[i].unHoverFunction();
                }
            }
        }
    }
}