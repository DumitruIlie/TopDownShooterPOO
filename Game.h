//Ilie Dumitru
#ifndef GAME_H
#define GAME_H

class Entity;

#include<SFML/Graphics.hpp>
#include<vector>
#include"Entity.h"

class Game
{
private:
    float dt;
    Entity* player;
    bool homingBullets;
    std::vector<Entity*> enemies;
    std::vector<Entity*> bullets;

    long long last_frame;
    long long last_bullet;

protected:
    virtual void gameLogic(sf::RenderWindow& window);
    virtual void handleMovement();
    virtual void handleShooting(sf::RenderWindow& window);
    virtual void trySpawnMonster(vec2f restrictedAreaCenter, float restrictedAreaRadius, float chancePerTick, sf::RenderWindow& window);

    Entity* spawnScout(vec2f spawnPos);
    Entity* spawnHeavy(vec2f spawnPos);

    virtual void restart(sf::RenderWindow& window);

    int score;

public:
    static const long long frame_time=3000000;
    static const long long bullet_time_spawn=200000000LL;
    static const long long invulnerabilityFramesTime=50000000LL;

    Game();
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    virtual ~Game();

    virtual void eventHandler(sf::RenderWindow& window);
    virtual void tick(sf::RenderWindow& window);
    virtual void render(sf::RenderWindow& window);

    Entity* getPlayer();
    Entity* getClosestEnemy(vec2f pos);

    bool isEnemyAlive(Entity* enemy);
};

#endif//GAME_H