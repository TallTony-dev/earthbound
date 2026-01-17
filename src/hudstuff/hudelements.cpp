#include "hudelements.hpp"
#include "hudanimmanager.hpp"
#include "hud.hpp"
#include <iostream>

HudElement::HudElement(Vector2 pos, float height, float width, float rot) {
    pos_ = ConvertToScreenPos(pos);
    height_ = height * hudScale;
    width_ = width * hudScale;
    rotation_ = rot;
    scale_ = 1;
}
void HudElement::ShowElement() {
    isHidden_ = false;
}
void HudElement::HideElement() {
    isHidden_ = true;
}
void HudElement::ToggleElementVis() {
    isHidden_ = !isHidden_;
}
void HudElement::Draw() {
    if (!isHidden_) {
        if (texture_.id != 0)
            DrawTexturePro(texture_,{0,0,(float)texture_.width,(float)texture_.height} ,{pos_.x,pos_.y,width_,height_}, {0, 0}, 0.0f, BLACK);
        else {
            DrawRectangleV({pos_.x, pos_.y}, {width_, height_}, BLACK);
        }

    }
}
void HudElement::Update(Vector2 hudMousePos) {}



                // #include <iostream>
                // float tim;
    bool HudElement::Intersects(Vector2 pos) {
    // tim += GetFrameTime();
    // if (tim > 1) {
    // std::cout << "mousex: " << pos.x << std::endl;
    // std::cout << "mousey: " << pos.y << std::endl;
    // std::cout << "x: " << pos_.x << std::endl;
    // std::cout << "y: " << pos_.y << std::endl;
    // std::cout << "botx: " << pos_.x + width_ << std::endl;
    // std::cout << "boty: " << pos_.y + height_ << std::endl;
    // std::cout << "realmousex: " << GetMouseX() << std::endl;
    // std::cout << "realmousey: " << GetMouseY() << std::endl;
    // tim = 0;
    // }
    if (pos.x > pos_.x && pos.x < pos_.x + width_ && pos.y > pos_.y && pos.y < pos_.y + height_)
        return true;
    else
        return false;
}

/// @brief Sets the height and width of the element to be scaled by a certain amount, grows from top left corner
void HudElement::SetScale(float scale) {
    float newScaleFactor = scale / scale_;
    width_ *= newScaleFactor;

    height_ *= newScaleFactor;
    scale_ = scale;
}
void HudElement::SetScaleFromCenter(float scale) {
    //should adjust width, height, and pos accordingly
    float newScaleFactor = scale / scale_;
    pos_.x -= (width_ * newScaleFactor - width_) / 2;
    pos_.y -= (height_ * newScaleFactor - height_) / 2;
    width_ *= newScaleFactor;
    height_ *= newScaleFactor;
    scale_ = scale;
}

Button::Button(Vector2 pos, float height, float width, float rot, HudAnimBehavior hoverBehavior, HudAnimBehavior onClickBehavior, std::function<void()> onClickFunction) : HudElement(pos, height, width, rot) {
    onClickFunction_ = onClickFunction;
    hoverBehavior_ = hoverBehavior;
    onClickBehavior_ = onClickBehavior;
}

/// @brief enables the whole element, both shows and activates 
void Button::EnableElement() {
    isActive_ = true;
    isHidden_ = false;
}
/// @brief counterpart to enableelement, hides and deactivates
void Button::DisableElement() {
    isActive_ = false;
    isHidden_ = true;
}
void Button::ActivateElement() {
    isActive_ = true;
}
void Button::DeactivateElement() {
    isActive_ = false;
}
void Button::ToggleElementActive() {
    isActive_ = !isActive_;
}


void Button::Update(Vector2 hudMousePos) {
    if (isActive_ && Intersects(hudMousePos)) {
        if (IsMouseButtonPressed(0)) {
            if (onClickFunction_ != 0)
                onClickFunction_();
            
            if (onClickBehavior_ == HudAnimBehavior::ExpandContractBounce) {
                AddHudAnimation(HudAnimation(this, 0.5, HudAnimBehavior::ExpandContractBounce, 1, 0.15));
            }
            // else if (hoverBehavior_ == HudAnimBehavior::Expand) {
            //     AddAnimToManager()
            // }
        }
    }
    else if (isActive_ && wasHovered_) {
        //undo hover animation
        if (hoverBehavior_ == HudAnimBehavior::ColorChange) {

        }
    }
}