//Ilie Dumitru
#include"Bullet.h"
#include"AppException.h"

Bullet::Bullet(float _damage, Game* _game, vec2f center, float radius) : Entity(_game, center, radius), damage(_damage)
{
    this->modifiables[SPEEDFACTOR]=5.5;
}

float Bullet::getDamage() const {return this->damage;}

BeeBullet::BeeBullet(Entity*_target, float _damage, Game* _game, const vec2f center, const float radius) : Bullet(_damage, _game, center, radius), target(_target), lastHomingAction(0) {}

void BeeBullet::tick(const float dt)
{
    if(!this->game->isEnemyAlive(this->target) || (this->target && this->lastHomingAction>=this->HOMINGWAITFRAMECOUNT))
    {
        this->lastHomingAction=0;
        try
        {
            this->target=this->game->getClosestEnemy(this->getCenter());
        }
        catch(InvalidQuery& ex)
        {
            this->target=0;
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