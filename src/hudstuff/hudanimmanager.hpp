#pragma once
#include "hudelements.hpp"
class HudElement; //maybe??

enum HudAnimBehavior{ ExpandContractBounce, LinExpand, LinContract, ColorChange };

void UpdateHudAnims();

struct HudAnimation {
    HudElement *element_;
    float duration_;
    float remainingDuration_;
    HudAnimBehavior behavior_;
    float initialValue_; //value to return to (scale, etc.)
    float finalValue_; //value to go to
    HudAnimation(HudElement *element, float duration, HudAnimBehavior behavior, float initValue, float finalValue) :
    element_(element), duration_(duration), remainingDuration_(duration), behavior_(behavior), initialValue_(initValue), finalValue_(finalValue) {}
};

void AddHudAnimation(HudAnimation anim);