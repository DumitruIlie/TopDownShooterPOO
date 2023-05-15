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

    void gameLogic(const sf::RenderWindow& window);
    void handleMovement();
    void handleShooting(const sf::RenderWindow& window);
    void trySpawnMonster(vec2f restrictedAreaCenter, float restrictedAreaRadius, float chancePerTick, const sf::RenderWindow& window);

    Entity* spawnScout(vec2f spawnPos);
    Entity* spawnHeavy(vec2f spawnPos);

    void restart(const sf::RenderWindow& window);

    int score;

public:
    static const long long frame_time=3000000;
    static const long long bullet_time_spawn=200000000LL;
    static const long long invulnerabilityFramesTime=50000000LL;

    Game();
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    ~Game();

    void eventHandler(sf::RenderWindow& window);
    void tick(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    [[nodiscard]] Entity& getPlayer() const;
    [[nodiscard]] Entity* getClosestEnemy(vec2f pos) const;

    bool isEnemyAlive(const Entity* enemy) const;
};

#endif//GAME_H