#ifndef gameh
#define gameh 0
#include "../raylib/src/raylib.h"

enum EntityShader {
    None = -1,
    Blorb = 0 
};

//enemy attacks
#define EATTACKJOLT 0
#define EATTACKJAB 1
#define EATTACKKICK 2
#define EATTACKSPIN 3

//player attacks
#define PATTACKDANCE 0
#define PATTACKSPAM 1

//void UseEnemyAttack(int attackid);
//void UsePlayerAttack(int buttonIndex);

//gamestate
#define MAINMENUSTATE 0
#define INMAPSTATE 1
#define INBATTLESTATE 2

void UpdateGame();
void DrawGame();
void InitializeGame();

int BattleEnemiesCount(); //gets number of active battle enemies
void UseAttackOnEnemy(int enemyIndex, int attackNum);


#endif //gameh