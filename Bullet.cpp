//Ilie Dumitru
#include"Bullet.h"
#include"AppException.h"
#include"Game.h"

Bullet::Bullet(const float _damage, Game* const _game, const vec2f center, const float radius) : Entity(_game, center, radius), damage(_damage)
{
    this->modifiables[SPEEDFACTOR]=5.5;
}

float Bullet::getDamage() const {return this->damage;}

BeeBullet::BeeBullet(Humanoid* const _target, const float _damage, Game* const _game, const vec2f center, const float radius) : Bullet(_damage, _game, center, radius), target(_target), lastHomingAction(0) {}

void BeeBullet::tick(const float dt)
{
    if(!this->game->isEnemyAlive(*this->target) || (this->target && this->lastHomingAction>=this->HOMINGWAITFRAMECOUNT))
    {
        this->lastHomingAction=0;
        try
        {
            this->target=&this->game->getClosestEnemy(this->getCenter());
        }
        catch(InvalidQuery&)
        {
            this->target=nullptr;
        }
    }
    else
        ++this->lastHomingAction;
    if(this->target)
    {
        this->setVelocity((this->getVelocity()+(this->target->getCenter()-this->getCenter()).normalize()*BeeBullet::HOMINGFACTOR).normalize());
    }
    Entity::tick(dt);
}