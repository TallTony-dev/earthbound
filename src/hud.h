#ifndef hudh
#define hudh 0
#include "../raylib/src/raylib.h"

#define HUDELEMENTCOUNT 4

#define HUD_ATTACKBTN 0
#define HUD_ITEMBTN 1
#define HUD_HEALTHBAR 2
#define HUD_SNOOZEBAR 3
#define HUD_STARTBTN 4


typedef struct hudelementstruct
{
    Vector2 pos;
    int height;
    int width;
    float rotation;
    Texture2D texture;
    bool isActive;
    bool isHidden;
    void (*onClickFunction)(int buttonIndex); //do something when clicked
    void (*onHoverFunction)(int buttonIndex); //do something when hovered
    void (*unHoverFunction)(int buttonIndex); //do something when unhovered
    bool wasHovered;
    int elementIndex;
    
} HudElement;

void CheckClick(Vector2 mousePos);
void CheckHover(Vector2 mousePos);
void InitializeHud();
void DrawHud(int windowHeight, int windowWidth);

//pass in macros to get indices
void DeactivateElement(int element);
void ActivateElement(int element);
void ToggleElement(int element);
void ShowElement(int element);
void HideElement(int element);
void ToggleElementVis(int element);

#endif //hudh