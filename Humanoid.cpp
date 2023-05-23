//Ilie Dumitru
#include"Humanoid.h"

Humanoid::Humanoid(Game* const _game, const vec2f center, const float radius, const float MAXHP, const bool overhealable) : Entity(_game, center, radius, MAXHP, overhealable) {}

Entity* Humanoid::clone() {return new Humanoid(*this);}