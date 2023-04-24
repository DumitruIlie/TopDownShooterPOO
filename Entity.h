//Ilie Dumitru
#ifndef ENTITY_H
#define ENTITY_H

#include"vec2.h"
#include<SFML/Graphics.hpp>

class Entity
{
private:
    vec2f center;
    const float radius;
    vec2f velocity;
    float hp;
    sf::Color color;

protected:

public:
    constexpr static const float MAX_HP=100;

    explicit Entity(const vec2f center=vec2f(), const float radius=1) : center(center), radius(radius), velocity(), hp(MAX_HP) {}
    virtual ~Entity() = default;

    [[nodiscard]] vec2f getCenter() const {return this->center;}
    void setCenter(const vec2f newCenter) {center=newCenter;}

    [[nodiscard]] float getRadius() const {return this->radius;}

    //vec2f getVelocity() const {return this->velocity;}
    void setVelocity(const vec2f newVelocity) {this->velocity=newVelocity;}

    void setColor(sf::Color _color) {this->color=_color;}

    void tick(const float dt) {this->center+=this->velocity*dt;}

    void draw(sf::RenderWindow& window) const;

    float takeDamage(float damage) {return this->hp-=damage;}

    void resetLife() {this->hp=MAX_HP;}

    friend std::ostream& operator<<(std::ostream& out, const Entity& circle) {return out<<'('<<circle.center<<", "<<circle.radius<<", "<<circle.velocity<<')';}
};

#endif//ENTITY_H