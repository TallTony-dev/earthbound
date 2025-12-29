#include "entity.hpp"

EntityShader Entity::GetShaderIndex() {
    return shader_;
}

Texture2D Entity::GetTexture() {
    return texture_;
}

float Entity::GetTimeWhenDamaged() {
    return timeWhenDamaged_;
}

void Entity::GetAttackedBy(int attackNum) {
    timeWhenDamaged_ = GetTime();
}