#include <unordered_map>
#include <string>
#include <iostream>
#include "../raylib/src/raylib.h"
#include "textures.hpp"

#define LoadTex(filename) (LoadTexture(TextFormat("%s%s%s", GetApplicationDirectory(), "../resources/textures/", filename)))

std::unordered_map<std::string_view, Texture2D> textures;

void InitializeTextures() {
    textures["rocks"] = LoadTex("rocks.png");
    SetTextureWrap(textures["rocks"], TEXTURE_WRAP_CLAMP);
}

Texture2D GetTexture(const char* key) {
    return textures[key];
}

void UnloadTextures() {
    for (auto& [key, value]: textures) {
        UnloadTexture(value);
    }
    textures.clear();
}
