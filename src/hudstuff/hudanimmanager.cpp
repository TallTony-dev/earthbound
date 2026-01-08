#include "hudelements.hpp"
#include "hudanimmanager.hpp"
#include <cmath>

std::vector<HudAnimation> activeanims;

void AddHudAnimation(HudAnimation anim) {
    activeanims.push_back(anim);
}



void UpdateHudAnims() {
    float deltaTime = GetFrameTime();
    for(auto i = activeanims.begin(); i != activeanims.end(); i++) {
        HudAnimation& anim = *i;
        if (anim.remainingDuration_ <= 0) {
            activeanims.erase(i);
            i--;
            continue;
        }

        if (anim.behavior_ == HudAnimBehavior::ExpandContractBounce) {
            if (anim.remainingDuration_ - deltaTime <= 0) {
                anim.element_->SetScaleFromCenter(anim.initialValue_);
                anim.remainingDuration_ -= deltaTime;
                continue;
            }
            float progress = (anim.duration_ - anim.remainingDuration_) / anim.duration_; // 0-1 depending on progress
            progress = (progress - 0.5) * 2; //between -1 and 1
            progress = 1 - std::abs(progress); //0-1-0 linearly
            float scale = anim.initialValue_ + progress * anim.finalValue_;
            anim.element_->SetScaleFromCenter(scale);
        }

        anim.remainingDuration_ -= deltaTime;
    }
}