#pragma once
#include "../raylib/src/raylib.h"
void InitializeTextures();
void UnloadTextures();
Texture2D GetTexture(const char* key);