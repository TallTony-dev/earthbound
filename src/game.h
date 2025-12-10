#ifndef gameh
#define gameh 0
#include "../raylib/src/raylib.h"

typedef struct {
    float health;
    float maxHealth;
    float defence;
    float attackPower;
    int attack1;
    int attack2;
    int attack3;
    int attack4; //default attack to -1 to not exist
    int typeIndex; //for textures/anims
    float timeWhenDamaged;
} Entity;
void InitializeEntity(Entity *entity, float health, float attackPow, float defence, int type, int attack1, int attack2, int attack3, int attack4);

//enemy types for textures/anims
#define ENEMYINDEXCOUNT 1
#define PLAYERTYPE -1
#define ENEMYGLEEBTYPE 0

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