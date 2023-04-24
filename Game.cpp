//Ilie Dumitru
#include"Game.h"
#include"MenuSystem.h"
#include"Physics.h"
#include"chrono"

Game::Game() : dt(1/32.f), player(vec2f(), 10), last_frame(std::chrono::system_clock::now().time_since_epoch().count()), last_bullet(std::chrono::system_clock::now().time_since_epoch().count()), score(0)
{
    this->player.setColor(sf::Color(0, 0, 255));
}

Game::~Game()
{
    int i;

    for(i=0;i<(int)this->enemies.size();++i)
        delete this->enemies[i];
    for(i=0;i<(int)bullets.size();++i)
        delete bullets[i];
}

Entity* Game::getPlayer() {return &this->player;}

void Game::spawnEnemy(Entity* enemy) {this->enemies.push_back(enemy);}

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

            if(Entity* monster=this->trySpawnMonster(this->player.getCenter(), 50, 0.0025f, window))
                this->spawnEnemy(monster);

            gameLogic(window);

            player.tick(dt);
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
            case sf::Event::MouseButtonPressed:
            {
                MENU prev=MenuSystem::getMenu();
                MenuSystem::click(vec2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
                if(MenuSystem::getMenu()==MAINMENU && prev==PAUSEMENU)
                    this->restart(window);
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
    if(!fontLoaded){
        if(!font.loadFromFile("arial.ttf")){
            throw std::runtime_error("Font could not be found, make sure it is in the right directory!\n");
        }
        fontLoaded=true;
    }

    if(MenuSystem::getMenu()==PAUSEMENU || MenuSystem::getMenu()==GAMEPLAY)
    {
        int i;

        player.draw(window);
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
        scoreText.setFillColor(sf::Color(255, 255, 255));
        scoreText.setFont(font);
        scoreText.setStyle(sf::Text::Bold);

        scoreText.setPosition(((float)window.getSize().x-(float)scoreText.getGlobalBounds().getSize().x)*0.5f, 10);
        window.draw(scoreText);
    }

    MenuSystem::render(window);
}

Entity* Game::trySpawnMonster(vec2f restrictedAreaCenter, float restrictedAreaRadius, float chance, sf::RenderWindow& window)
{
    if((float)rand()/(float)RAND_MAX<chance)
    {
        vec2f pos(((float)rand()/(float)RAND_MAX)*(float)window.getSize().x,((float)rand()/(float)RAND_MAX)*(float)window.getSize().y);
        while((pos-restrictedAreaCenter).lengthSquared()<restrictedAreaRadius*restrictedAreaRadius)
            pos=vec2f(((float)rand()/(float)RAND_MAX)*(float)window.getSize().x,((float)rand()/(float)RAND_MAX)*(float)window.getSize().y);
        auto monster=new Entity(pos,9);
        monster->setColor(sf::Color(255,0,0));
        return monster;
    }
    return nullptr;
}

void Game::gameLogic(sf::RenderWindow& window)
{
    int i, j;
    for(i=0;i<(int)this->enemies.size();++i)
    {
        if(Physics::checkIntersection(&this->player, this->enemies[i]))
        {
            if(this->player.takeDamage(5)<=0)
            {
                MenuSystem::setMenu(GAMEOVER);
                this->restart(window);
                return;
            }
        }
        else
            this->enemies[i]->setVelocity((this->player.getCenter()-this->enemies[i]->getCenter()).normalize()*=2);

        for(j=0;j<(int)this->bullets.size();++j)
        {
            if(Physics::checkIntersection(this->enemies[i], this->bullets[j]))
            {
                std::swap(this->bullets[j], this->bullets[(int)this->bullets.size()-1]);
                delete this->bullets[(int)this->bullets.size()-1];
                this->bullets.pop_back();
                --j;
                if(this->enemies[i]->takeDamage(20)<=0)
                {
                    std::swap(this->enemies[i], this->enemies[(int)this->enemies.size()-1]);
                    delete this->enemies[(int)this->enemies.size()-1];
                    this->enemies.pop_back();
                    --i;
                    j=(int)bullets.size();
                    ++this->score;
                }
            }
        }
    }
}

void Game::restart(sf::RenderWindow& window)
{
    int i;

    this->score=0;
    this->player.setVelocity(vec2f());
    this->player.setCenter(vec2f((float)window.getSize().x, (float)window.getSize().y)/2.f);
    for(i=0;i<(int)this->bullets.size();++i)
        delete this->bullets[i];
    for(i=0;i<(int)this->enemies.size();++i)
        delete this->enemies[i];
    this->bullets.clear();
    this->enemies.clear();
}

void Game::handleMovement()
{
    this->player.setVelocity(vec2f((float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)),
                                            -(float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)))
                                          .normalize()*=4);
}

void Game::handleShooting(sf::RenderWindow& window)
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && std::chrono::system_clock::now().time_since_epoch().count()-this->last_bullet>=bullet_time_spawn)
    {
        this->last_bullet=std::chrono::system_clock::now().time_since_epoch().count();
        auto bullet=new Entity(this->player.getCenter(),2);
        sf::Vector2i mousePos=sf::Mouse::getPosition(window);
        vec2f vel=(vec2f((float)mousePos.x,(float)mousePos.y)-this->player.getCenter());
        float len=vel.length();
        if(len<=1e-9)
            vel=vec2f(0, 1);
        else
            vel*=11/len;
        bullet->setVelocity(vel);
        bullet->setColor(sf::Color(255,255,0));
        this->bullets.push_back(bullet);
    }
}