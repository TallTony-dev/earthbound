#include "hud.h"
#include "../raylib/src/raymath.h"

HudElement elements[HUDELEMENTCOUNT];

void InitializeHud() {

    HudElement elements_[HUDELEMENTCOUNT] = { 
        #include "hudembed.txt" 
    };
    for (int i = 0; i < HUDELEMENTCOUNT; i++) {
        elements[i] = elements_[i];
        elements[i].elementIndex = i;
    }\
}

void DrawHud() {
    float xscale = GetScreenWidth() / HUDWIDTH;
    float yscale = GetScreenHeight() / HUDHEIGHT;

    for (int i = 0; i < HUDELEMENTCOUNT; i++) {
        if (!elements[i].isHidden) {
            if (elements[i].texture.id != 0)
                DrawTexturePro(elements[i].texture, (Rectangle){0,0,0,0}, (Rectangle) {elements[i].pos.x * xscale, elements[i].pos.y * yscale, elements[i].width * xscale, elements[i].height * yscale}, (Vector2){}, elements[i].rotation, WHITE);
            else
                DrawRectangleV((Vector2) {elements[i].pos.x * xscale, elements[i].pos.y * yscale}
                ,(Vector2) {elements[i].width * xscale, elements[i].height * yscale}, DARKBLUE);
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
                if (mousePos.x > elements[i].pos.x && mousePos.x < elements[i].pos.x + elements[i].width && mousePos.y > elements[i].pos.y && mousePos.y < elements[i].pos.y + elements[i].height)
                    elements[i].onClickFunction(elements[i].elementIndex);
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
                    elements[i].onHoverFunction(i);
                    elements[i].wasHovered = true;
                }
                else if (!intersects && elements[i].wasHovered) {
                    elements[i].wasHovered = false;
                    elements[i].unHoverFunction(i);
                }
            }
        }
    }
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