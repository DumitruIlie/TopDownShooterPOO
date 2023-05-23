//Ilie Dumitru
#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include<vector>
#include"Bullet.h"
#include"Humanoid.h"

class Game
{
private:
    float dt;
    Humanoid player;
    int bulletType;
    std::vector<Humanoid*> enemies;
    std::vector<Bullet*> bullets;

    long long last_frame;
    long long last_bullet;

    void gameLogic(const sf::RenderWindow& window);
    void handleMovement();
    void handleShooting(const sf::RenderWindow& window);
    void trySpawnMonster(vec2f restrictedAreaCenter, float restrictedAreaRadius, float chancePerTick, const sf::RenderWindow& window);

    Humanoid* spawnScout(vec2f spawnPos);
    Humanoid* spawnHeavy(vec2f spawnPos);

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

    [[nodiscard]] Humanoid& getPlayer();
    [[nodiscard]] Humanoid& getClosestEnemy(vec2f pos) const;

    void addBullet(Bullet* bullet);

    [[nodiscard]] bool isEnemyAlive(const Humanoid& enemy) const;
};

#endif//GAME_H