#pragma once
#include <vector>
#include "hudelements.hpp"



class Menu
{
    public:
        virtual void Update(Vector2 mousePos);
        void Activate();
        void Deactivate();
        bool IsActive();
        void Draw();
    protected:
        std::vector<HudElement*> elements;
        bool isActive_; //controls drawing and updating in this case
};


#define BOUTOFBATTLE -1
#define BPLAYERTURN 0
#define BITEMSCREEN 1
#define BATTACKSCREEN 2
#define BINACTION 3
class BattleMenu : public Menu {
    public:
        BattleMenu();
        void Update(Vector2 mousePos) override;
        void SetBattleState(int BattleState);
    private:  
        int hudBattleState = BOUTOFBATTLE;
        int prevHudBattleState = BOUTOFBATTLE;
};