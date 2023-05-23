//Ilie Dumitru
#ifndef HUMANOID_H
#define HUMANOID_H

#include"Entity.h"

class Humanoid : public Entity
{
public:
    friend class EnemyBuilder;

    explicit Humanoid(Game* _game, vec2f center=vec2f(), float radius=1, float MAXHP=100, bool overhealable=true);

    Entity* clone() override;
};

#endif//HUMANOID_H