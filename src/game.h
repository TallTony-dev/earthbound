#ifndef gameh
#define gameh 0
#include "../raylib/src/raylib.h"

//gamestate
#define MAINMENU 0
#define INMAP 1
#define INBATTLE 2


void UpdateGame();
void DrawGame(int windowHeight, int windowWidth);
void InitializeGame();
#endif //gameh