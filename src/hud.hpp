#ifndef hudh
#define hudh 0
#include "../raylib/src/raylib.h"
#include <functional>

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



class HudElement
{
    public: //transitioning from a struct so everything is public for now :/ sorry gang
        Vector2 pos;
        float height;
        float width;
        float rotation;
        Texture2D texture;
        bool isActive;
        bool isHidden;
        std::function<void()> onClickFunction; //do something when clicked
        std::function<void()> onHoverFunction; //do something when hovered
        std::function<void()> unHoverFunction; //do something when unhovered
        bool wasHovered;
        int elementIndex;
    
};

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