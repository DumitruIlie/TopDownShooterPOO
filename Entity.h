//Ilie Dumitru
#ifndef ENTITY_H
#define ENTITY_H

enum MODIFIABLE{SPEEDFACTOR, DAMAGEFACTOR, HEALTH, CNTMODIFIABLES};

class Game;

#include"vec2.h"
#include"Powerup.h"
#include<SFML/Graphics.hpp>

class Entity
{
public:
    vec2f center;
    float radius;
    vec2f velocity;
    float hp;
    float MAX_HP;
    sf::Color color;

private:
    long long lastDamageTaken;

protected:
    Game* game;
    float modifiables[CNTMODIFIABLES];
    bool overhealable;

public:
    explicit Entity(Game* _game, vec2f center=vec2f(), float radius=1, float MAXHP=100, bool overhealable=true);
    Entity(const Entity& other) = default;
    virtual ~Entity() = default;

    [[nodiscard]] vec2f getCenter() const;
    void setCenter(vec2f newCenter);

    [[nodiscard]] float getRadius() const;

    [[nodiscard]] vec2f getVelocity() const;
    void setVelocity(vec2f newVelocityNormal);

    void setColor(sf::Color _color);

    virtual void tick(float dt);

    void draw(sf::RenderWindow& window) const;

    float takeDamage(float damage);

    [[nodiscard]] float getHP() const;

    void resetLife();

    friend std::ostream& operator<<(std::ostream& out, const Entity& circle);

    [[nodiscard]] float getModifiable(MODIFIABLE modif) const;

    virtual Entity* clone();

    void setModifiable(MODIFIABLE modif, bool isAdditive, float value);
};

#endif//ENTITY_H