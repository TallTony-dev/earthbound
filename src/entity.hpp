#ifndef ENTITY
#define ENTITY 0

#include "game.hpp"

class Entity {
    private:
        float health_;
        float maxHealth_;
        float defence_;
        float attackPower_;
        int attack1_;
        int attack2_;
        int attack3_;
        int attack4_; //default attack to -1 to not exist
        float timeWhenDamaged_;
        EntityShader shader_;
        Texture2D texture_;

    public:
        Entity(float health, float attackPow, float defence, int attack1, int attack2 = -1, int attack3 = -1, int attack4 = -1) :
        health_(health), attackPower_(attackPow), defence_(defence), attack1_(attack1), attack2_(attack2), attack3_(attack3), attack4_(attack4) {};
        EntityShader GetShaderIndex();
        Texture2D GetTexture();
        float GetTimeWhenDamaged();
        void GetAttackedBy(int attackNum);
};

#endif