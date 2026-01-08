#pragma once
#include "../../raylib/src/raylib.h"
#include "hudanimmanager.hpp"
#include <functional>

class HudElement
{
    public: 
        virtual void Update(Vector2 hudMousePos);
        void Draw();
        void ShowElement();
        void HideElement();
        void ToggleElementVis();
        void SetScale(float scale);
        void SetScaleFromCenter(float scale); //scales size but keeps center in same place
        HudElement(Vector2 pos, float height, float width, float rot);
    protected:
        Vector2 pos_;
        float scale_;
        float height_;
        float width_;
        float rotation_;
        Texture2D texture_;
        bool isHidden_;
        bool wasHovered_;
        bool Intersects(Vector2 pos);
};

class Button : public HudElement {
    public:
        Button(Vector2 pos, float height, float width, float rot, HudAnimBehavior hoverBehavior, HudAnimBehavior onClickBehavior, std::function<void()> onClickFunction = 0);
        void Update(Vector2 hudMousePos) override;
        void EnableElement(); //enable visual and functional
        void DisableElement();//disable visual and functional
        void ActivateElement();
        void DeactivateElement();
        void ToggleElementActive();
    private:
        bool isActive_;
        std::function<void()> onClickFunction_; //0 = null
        HudAnimBehavior hoverBehavior_;
        HudAnimBehavior onClickBehavior_;
};