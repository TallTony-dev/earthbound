#ifndef hudh
#define hudh 0
#include "../raylib/src/raylib.h"

#define HUDWIDTH 1600.0f
#define HUDHEIGHT 900.0f

#define HUDELEMENTCOUNT 9

//B denotes battle elements
//M denotes main menu elements
#define HUD_BATTACKBTN 0
#define HUD_BITEMBTN 1
#define HUD_BHEALTHBAR 2
#define HUD_BSNOOZEBAR 3
#define HUD_MSTARTBTN 4
#define HUD_BATTACK1BTN 5
#define HUD_BATTACK2BTN 6
#define HUD_BATTACK3BTN 7
#define HUD_BATTACK4BTN 8



typedef struct hudelementstruct
{
    Vector2 pos;
    int height;
    int width;
    float rotation;
    Texture2D texture;
    bool isActive;
    bool isHidden;
    int onClickParam1;
    int onClickParam2;
    void *onClickFunction; //do something when clicked
    void (*onHoverFunction)(int buttonIndex); //do something when hovered
    void (*unHoverFunction)(int buttonIndex); //do something when unhovered
    bool wasHovered;
    int elementIndex;
    
} HudElement;

void CheckClick(Vector2 mousePos);
void CheckHover(Vector2 mousePos);
void InitializeHud();
void UpdateHud();
void DrawHud();

void ExitBattleState();
void EnterBattleState();

//pass in macros to get indices
//void DeactivateElement(int element);
//void ActivateElement(int element);
//void ToggleElement(int element);
//void ShowElement(int element);
//void HideElement(int element);
//void ToggleElementVis(int element);

#endif //hudh