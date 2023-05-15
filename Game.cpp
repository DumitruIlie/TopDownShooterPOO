//Ilie Dumitru
#include"Game.h"
#include"Bullet.h"
#include"MenuSystem.h"
#include"Physics.h"
#include"chrono"
#include"AppException.h"
#include"EnemyBuilder.h"

Game::Game() : dt(1/32.f), player(new Entity(this, vec2f(), 10)), homingBullets(false), last_frame(std::chrono::system_clock::now().time_since_epoch().count()), last_bullet(std::chrono::system_clock::now().time_since_epoch().count()), score(0)
{
    this->player->setColor(sf::Color(0, 0, 255));
}

Game::~Game()
{
    int i;

    delete this->player;
    for(i=0;i<(int)this->enemies.size();++i)
        delete this->enemies[i];
    for(i=0;i<(int)bullets.size();++i)
        delete bullets[i];
}

Entity& Game::getPlayer() {return *this->player;}

void Game::tick(sf::RenderWindow& window)
{
    long long currFrame=std::chrono::system_clock::now().time_since_epoch().count();
    if(currFrame-this->last_frame>=frame_time)
    {
        this->last_frame+=frame_time;

        this->eventHandler(window);

        if(MenuSystem::getMenu()==GAMEPLAY)
        {
            int i;

            this->handleMovement();
            this->handleShooting(window);

            this->trySpawnMonster(this->player->getCenter(), 50, 0.0025f, window);

            gameLogic(window);

            player->tick(dt);
            for(i=0;i<(int)this->enemies.size();++i)
                this->enemies[i]->tick(dt);
            for(i=0;i<(int)this->bullets.size();++i)
                this->bullets[i]->tick(dt);
        }
    }
}

void Game::eventHandler(sf::RenderWindow& window)
{
    sf::Event e=sf::Event();
    while(window.pollEvent(e))
    {
        switch(e.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
            {
                if(e.key.code==sf::Keyboard::Z)
                    this->homingBullets=false;
                if(e.key.code==sf::Keyboard::X)
                    this->homingBullets=true;
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if(e.mouseButton.button==sf::Mouse::Left)
                {
                    MENU prev=MenuSystem::getMenu();
                    MenuSystem::click(vec2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
                    if(MenuSystem::getMenu()==MAINMENU && prev==PAUSEMENU)
                        this->restart(window);
                }
                break;
            }
            default:
                break;
        }
    }
}

void Game::render(sf::RenderWindow& window)
{
    static sf::Font font;
    static bool fontLoaded=false;
    if(!fontLoaded)
    {
        if(!font.loadFromFile("../resurse/arial.ttf"))
        {
            //throw std::runtime_error("Font could not be found, make sure it is in the right directory!\n");
            throw MissingFontException("Font could not be found where expected!\n");
        }
        fontLoaded=true;
    }

    if(MenuSystem::getMenu()==PAUSEMENU || MenuSystem::getMenu()==GAMEPLAY)
    {
        int i;

        player->draw(window);
        for(i=0;i<(int)this->enemies.size();++i)
            this->enemies[i]->draw(window);
        for(i=0;i<(int)this->bullets.size();++i)
        {
            vec2f pos_bullet=this->bullets[i]->getCenter();
            float rad=this->bullets[i]->getRadius();
            if(pos_bullet.get_x()<-rad || pos_bullet.get_x()>(float)window.getSize().x+rad || pos_bullet.get_y()<-rad || pos_bullet.get_y()>(float)window.getSize().y+rad)
            {
                std::swap(this->bullets[i], this->bullets[(int)this->bullets.size()-1]);
                this->bullets.pop_back();
            }
            else
                this->bullets[i]->draw(window);
        }

        sf::Text scoreText;
        char scoreChar[15];
        snprintf(scoreChar, 10, "%d", this->score*10);
        scoreText.setString(scoreChar);
        scoreText.setCharacterSize(34);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setFont(font);
        scoreText.setStyle(sf::Text::Bold);

        scoreText.setPosition(((float)window.getSize().x-(float)scoreText.getGlobalBounds().getSize().x)*0.5f, 10);
        window.draw(scoreText);

        snprintf(scoreChar, 10, "%d", (int)this->getPlayer().getHP());
        scoreText.setString(scoreChar);

        scoreText.setPosition(((float)window.getSize().x-(float)scoreText.getGlobalBounds().getSize().x)*0.5f, 50);
        window.draw(scoreText);
    }

    MenuSystem::render(window);
}

Entity* Game::spawnScout(vec2f spawnPos)
{
    EnemyBuilder builder(this);
    return builder.setPos(spawnPos).setMaxHealth(100, false).setSize(7).setColor(sf::Color(0, 255, 0)).applyPowerup(SpeedIncreasePowerup(1)).applyPowerup(DamageIncreasePowerup(-0.5f)).spawn();
}

Entity* Game::spawnHeavy(vec2f spawnPos)
{
    EnemyBuilder builder(this);
    return builder.setPos(spawnPos).setMaxHealth(300, false).setSize(15).setColor(sf::Color(255, 255, 0)).applyPowerup(SpeedIncreasePowerup(-0.25f)).applyPowerup(DamageIncreasePowerup(3)).spawn();
}

void Game::trySpawnMonster(vec2f restrictedAreaCenter, float restrictedAreaRadius, float chance, sf::RenderWindow& window)
{
    if((float)rand()/(float)RAND_MAX<chance)
    {
        vec2f pos(((float)rand()/(float)RAND_MAX)*(float)window.getSize().x,((float)rand()/(float)RAND_MAX)*(float)window.getSize().y);
        while((pos-restrictedAreaCenter).lengthSquared()<restrictedAreaRadius*restrictedAreaRadius)
            pos=vec2f(((float)rand()/(float)RAND_MAX)*(float)window.getSize().x,((float)rand()/(float)RAND_MAX)*(float)window.getSize().y);

        float typeSpawn=(float)rand()/(float)RAND_MAX;
        if(typeSpawn<0.111)
            this->enemies.push_back(spawnScout(pos));
        else if(typeSpawn<0.222)
            this->enemies.push_back(spawnHeavy(pos));
        else
        {
            EnemyBuilder builder(this);
            this->enemies.push_back(builder.setColor(sf::Color(255,0,0)).setSize(9).setPos(pos).setMaxHealth(150, false).spawn());
        }
    }
}

void Game::gameLogic(sf::RenderWindow& window)
{
    int i, j;
    for(i=0;i<(int)this->enemies.size();++i)
    {
        if(Physics::checkIntersection(this->player, this->enemies[i]))
        {
            if(this->player->takeDamage(this->enemies[i]->getDamage())<=0)
            {
                MenuSystem::setMenu(GAMEOVER);
                this->restart(window);
                return;
            }
        }
        else
            this->enemies[i]->setVelocity((this->player->getCenter()-this->enemies[i]->getCenter()).normalize());

        for(j=0;j<(int)this->bullets.size();++j)
        {
            if(Physics::checkIntersection(this->enemies[i], this->bullets[j]))
            {
                int next_j=j-1;
                if(this->enemies[i]->takeDamage(dynamic_cast<Bullet*>(bullets[j])->getDamage())<=0)
                {
                    std::swap(this->enemies[i], this->enemies[(int)this->enemies.size()-1]);
                    delete this->enemies[(int)this->enemies.size()-1];
                    this->enemies.pop_back();
                    --i;
                    next_j=(int)bullets.size();
                    ++this->score;

                    //Testing code
                    HealthIncreasePowerup hp_inc(5);
                    hp_inc.affectEntity(this->player);
                }
                std::swap(this->bullets[j], this->bullets[(int)this->bullets.size()-1]);
                delete this->bullets[(int)this->bullets.size()-1];
                this->bullets.pop_back();
                j=next_j;
            }
        }
    }
}

void Game::restart(sf::RenderWindow& window)
{
    int i;

    this->score=0;
    this->player->setVelocity(vec2f());
    this->player->resetLife();
    this->player->setCenter(vec2f((float)window.getSize().x, (float)window.getSize().y)/2.f);
    for(i=0;i<(int)this->bullets.size();++i)
        delete this->bullets[i];
    for(i=0;i<(int)this->enemies.size();++i)
        delete this->enemies[i];
    this->bullets.clear();
    this->enemies.clear();
}

void Game::handleMovement()
{
    this->player->setVelocity(vec2f((float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)),
                                             -(float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)))
                                             .normalize()*=4);
}

void Game::handleShooting(sf::RenderWindow& window)
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && std::chrono::system_clock::now().time_since_epoch().count()-this->last_bullet>=bullet_time_spawn)
    {
        this->last_bullet=std::chrono::system_clock::now().time_since_epoch().count();
        Entity* bullet;
        if(this->homingBullets)
            bullet=new BeeBullet(nullptr, 25, this, this->player->getCenter(), 2);
        else
            bullet=new Bullet(50, this, this->player->getCenter(), 2);
        sf::Vector2i mousePos=sf::Mouse::getPosition(window);
        vec2f vel=(vec2f((float)mousePos.x,(float)mousePos.y)-this->player->getCenter());
        float len=vel.length();
        if(len<=1e-9)
            vel=vec2f(0, 1);
        else
            vel/=len;
        bullet->setVelocity(vel);
        bullet->setColor(sf::Color(255,255,0));
        this->bullets.push_back(bullet);
    }
}

bool Game::isEnemyAlive(const Entity* const enemy)
{
    if(!enemy)
        return false;
    int i;
    for(i=0;i<(int)this->enemies.size();++i)
        if(this->enemies[i]==enemy)
            return true;
    return false;
}

Entity* Game::getClosestEnemy(vec2f pos)
{
    if((int)this->enemies.size()==0)
        throw InvalidQuery("");
    int i, min=0;
    float mindist=(pos-this->enemies[0]->getCenter()).lengthSquared();
    for(i=1;i<(int)this->enemies.size();++i)
    {
        float aux=(pos-this->enemies[i]->getCenter()).lengthSquared();
        if(aux<mindist)
        {
            mindist=aux;
            min=i;
        }
    }
    return this->enemies[min];
}