//Ilie Dumitru
#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include<vector>
#include"Entity.h"

class Game
{
private:
    float dt;
    Entity player;
    std::vector<Entity*> enemies, bullets;

    long long last_frame;
    long long last_bullet;

protected:
    virtual void gameLogic(sf::RenderWindow& window);
    virtual void handleMovement();
    virtual void handleShooting(sf::RenderWindow& window);
    virtual Entity* trySpawnMonster(vec2f restrictedAreaCenter, float restrictedAreaRadius, float chancePerTick, sf::RenderWindow& window);

    virtual void restart(sf::RenderWindow& window);

    int score;

public:
    static const long long frame_time=3000000;
    static const long long bullet_time_spawn=200000000LL;

    Game();
    virtual ~Game();

    virtual void eventHandler(sf::RenderWindow& window);
    virtual void tick(sf::RenderWindow& window);
    virtual void render(sf::RenderWindow& window);

    Entity* getPlayer();
    void spawnEnemy(Entity* enemy);
};

#endif//GAME_H