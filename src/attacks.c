#include "game.h"
#include "attacks.h"

void AttackEnemy(Entity *toAttack, int attackIndex) {
    toAttack->timeWhenDamaged = GetTime();
}
void AttackPlayer(Entity *player,int attackIndex) {
    player->timeWhenDamaged = GetTime();
}