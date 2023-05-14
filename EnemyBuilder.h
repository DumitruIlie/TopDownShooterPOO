//Ilie Dumitru
#ifndef ENEMYBUILDER_H
#define ENEMYBUILDER_H

#include"Entity.h"
#include<SFML/Graphics.hpp>

class EnemyBuilder
{
private:
    Entity* beingBuilt;
    Game* game;

protected:

public:
    explicit EnemyBuilder(Game* _game);
    EnemyBuilder(const EnemyBuilder& other);
    EnemyBuilder(EnemyBuilder&& other) noexcept;
    EnemyBuilder& operator=(const EnemyBuilder& other);
    EnemyBuilder& operator=(EnemyBuilder&& other) noexcept;
    ~EnemyBuilder();

    void checkMissing();
    EnemyBuilder& setPos(vec2f pos);
    EnemyBuilder& setSize(float radius);
    EnemyBuilder& setInitVelocity(vec2f vel);
    EnemyBuilder& setMaxHealth(float max_hp, bool overhealable);
    EnemyBuilder& setColor(sf::Color col);
    EnemyBuilder& setGame(Game* _game);
    EnemyBuilder& applyPowerup(const Powerup& powerup);

    Entity* spawn();
};

#endif//ENEMYBUILDER_H