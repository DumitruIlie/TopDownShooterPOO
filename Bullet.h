//Ilie Dumitru
#ifndef BULLET_H
#define BULLET_H

#include"Entity.h"
#include"Humanoid.h"

class Bullet : public Entity
{
private:
    float damage;

protected:

public:
    Bullet(float _damage, Game* _game, vec2f center=vec2f(), float radius=2);
    [[nodiscard]] float getDamage() const;
};

class BeeBullet : public Bullet
{
private:
    Humanoid* target;
    int lastHomingAction;

public:
    const float HOMINGFACTOR=0.1f;
    const int HOMINGWAITFRAMECOUNT=200;
    BeeBullet(Humanoid* _target, float _damage, Game* _game, vec2f center=vec2f(), float radius=2);

    void tick(float dt) override;
};

#endif//BULLET_H