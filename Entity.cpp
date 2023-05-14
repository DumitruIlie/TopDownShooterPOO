//Ilie Dumitru
#include"Entity.h"
#include<chrono>

Entity::Entity(Game*_game, const vec2f center, const float radius, const float MAXHP, const bool _overhealable) : center(center), radius(radius), velocity(), hp(MAXHP), MAX_HP(MAXHP*(1+0.5f*(float)_overhealable)), lastDamageTaken(0), game(_game), modifiables(), overhealable(_overhealable)
{
    int i;
    for(i=0;i<CNTMODIFIABLES;++i)
        this->modifiables[i]=1;
}

vec2f Entity::getCenter() const {return this->center;}

void Entity::setCenter(const vec2f newCenter) {center=newCenter;}

float Entity::getRadius() const {return this->radius;}

[[nodiscard]] vec2f Entity::getVelocity() const {return this->velocity;}

void Entity::setVelocity(const vec2f newVelocityNormal) {this->velocity=newVelocityNormal*this->modifiables[SPEEDFACTOR];}

void Entity::setColor(sf::Color _color) {this->color=_color;}

void Entity::tick(const float dt) {this->center+=this->velocity*dt*this->getModifiable(SPEEDFACTOR);}

void Entity::draw(sf::RenderWindow&window) const
{
    sf::CircleShape shape(this->radius);
    shape.setPosition(sf::Vector2f(this->center.get_x()-this->radius, this->center.get_y()-this->radius));
    shape.setFillColor(this->color);
    window.draw(shape);
}

float Entity::takeDamage(float damage)
{
    long long curr=std::chrono::system_clock::now().time_since_epoch().count();
    if(curr-this->lastDamageTaken>Game::invulnerabilityFramesTime)
    {
        if(damage>0)
            this->lastDamageTaken=curr;
        this->hp-=damage;
        if(this->hp>this->MAX_HP)
            this->hp=this->MAX_HP;
        return this->hp;
    }
    return this->hp;
}

float Entity::getHP() const {return this->hp;}

void Entity::resetLife() {this->hp=MAX_HP/(1+0.5f*(float)this->overhealable);}

std::ostream& operator<<(std::ostream& out, const Entity& circle) {return out<<'('<<circle.center<<", "<<circle.radius<<", "<<circle.velocity<<')';}

float Entity::getModifiable(MODIFIABLE modif) const {return this->modifiables[modif];}

float Entity::getDamage() const {return 5*this->getModifiable(DAMAGEFACTOR);}

Entity* Entity::clone()
{
    return new Entity(*this);
}