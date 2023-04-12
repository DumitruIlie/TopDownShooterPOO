//Ilie Dumitru
#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<exception>
#include<cmath>
#include<chrono>
#include<cstdlib>
#include<vector>

int WINDOWWIDTH, WINDOWHEIGHT;
const long long bullet_time_spawn=200000000LL;

template <class T>
class vec2
{
private:
    T x, y;

public:
    vec2() : x(), y() {}
    vec2(T _x, T _y) : x(_x), y(_y) {}

    vec2<T> operator+(const vec2<T>& v) const {return vec2<T>(this->x+v.x, this->y+v.y);}
    vec2<T> operator-(const vec2<T>& v) const {return vec2<T>(this->x-v.x, this->y-v.y);}
    vec2<T> operator-() const {return vec2<T>(-this->x, -this->y);}
    vec2<T> operator*(const T& s) const {return vec2<T>(this->x*s, this->y*s);}
    friend vec2<T> operator*(const T& s, const vec2<T>& a) {return vec2<T>(a.x*s, a.y*s);}
    vec2<T> operator/(const T& s) const {if(s) return vec2<T>(this->x/s, this->y/s); return vec2<T>();}

    vec2<T>& operator+=(const vec2<T>& v) {this->x+=v.x; this->y+=v.y; return *this;}
    vec2<T>& operator-=(const vec2<T>& v) {this->x-=v.x; this->y-=v.y; return *this;}
    vec2<T>& operator*=(const T& s) {this->x*=s; this->y*=s; return *this;}
    vec2<T>& operator/=(const T& s) {if(s) {this->x/=s; this->y/=s;} else {this->x=this->y=0;} return *this;}

    T operator*(const vec2<T>& v) const {return this->x*v.x+this->y*v.y;}
    T operator^(const vec2<T>& v) const {return this->x*v.y-this->x*v.y;}
    bool operator==(const vec2<T>& v) const {return this->x==v.x && this->y==v.y;}
    bool operator!=(const vec2<T>& v) const {return this->x!=v.x || this->y!=v.y;}

    friend std::ostream& operator<<(std::ostream& out, const vec2<T>& v) {return out<<'('<<v.x<<", "<<v.y<<')';}
    friend std::istream& operator>>(std::istream& in, const vec2<T>& v) {return in>>v.x>>v.y;}

    [[nodiscard]] T get_x() const {return this->x;}
    [[nodiscard]] T get_y() const {return this->y;}
    //void set_x(const T& _x) {this->x=_x;}
    //void set_y(const T& _y) {this->y=_y;}

    [[nodiscard]] T lengthSquared() const {return this->x*this->x+this->y*this->y;}
    [[nodiscard]] T length() const {return sqrt(this->x*this->x+this->y*this->y);}
    vec2<T>& normalize() {return *this/=this->length();}
    //[[nodiscard]] vec2<T> normalized() const {return *this/this->length();}
};
typedef vec2<int> vec2i;
typedef vec2<float> vec2f;

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
    explicit Entity(const vec2f center=vec2f(), const float radius=1) : center(center), radius(radius), velocity(), hp(100) {}
    virtual ~Entity() = default;

    [[nodiscard]] vec2f getCenter() const {return this->center;}
    void setCenter(const vec2f newCenter) {center=newCenter;}

    [[nodiscard]] float getRadius() const {return this->radius;}

    //vec2f getVelocity() const {return this->velocity;}
    void setVelocity(const vec2f newVelocity) {this->velocity=newVelocity;}

    void setColor(sf::Color _color) {this->color=_color;}

    void tick(const float dt) {this->center+=this->velocity*dt;}

    void draw(sf::RenderWindow& window) const {sf::CircleShape shape(this->radius); shape.setPosition(sf::Vector2f(this->center.get_x()-this->radius, this->center.get_y()-this->radius)); shape.setFillColor(this->color); window.draw(shape);}

    float takeDamage(float damage) {return this->hp-=damage;}

    friend std::ostream& operator<<(std::ostream& out, const Entity& circle) {return out<<'('<<circle.center<<", "<<circle.radius<<", "<<circle.velocity<<')';}
};

class Physics
{
public:
    Physics() = delete;

    static bool checkIntersection(Entity* A, Entity* B) {return (A->getCenter()-B->getCenter()).lengthSquared()<=(A->getRadius()+B->getRadius())*(A->getRadius()+B->getRadius());}
};

class Button
{
private:
    void (*onClick)();
    void (*drawFnc)(Button&, bool, sf::RenderWindow&);

protected:
    vec2i topLeft, bottomRight;

public:
    Button() : onClick(nullptr), drawFnc(nullptr), topLeft(0, 0), bottomRight(0, 0) {}
    Button(vec2i _topLeft, vec2i _bottomRight) : onClick(nullptr), drawFnc(nullptr), topLeft(_topLeft), bottomRight(_bottomRight) {}
    Button(vec2i _topLeft, vec2i _bottomRight, void (*_onClick)(), void (*_drawFnc)(Button&, bool, sf::RenderWindow&)) : onClick(_onClick), drawFnc(_drawFnc), topLeft(_topLeft), bottomRight(_bottomRight) {}
    Button(const Button& other) = default;
    virtual ~Button() = default;
    Button& operator=(const Button& other) = default;

    void click(vec2i mousePos) {if(this->topLeft.get_x()<=mousePos.get_x() && mousePos.get_x()<=this->bottomRight.get_x() && this->topLeft.get_y()<=mousePos.get_y() && mousePos.get_y()<=this->bottomRight.get_y() && this->onClick) this->onClick();}
    void draw(sf::RenderWindow& window)
    {
        bool hover=(sf::Mouse::getPosition(window).x>=this->topLeft.get_x() && sf::Mouse::getPosition(window).x<=this->bottomRight.get_x() && sf::Mouse::getPosition(window).y>=this->topLeft.get_y() && sf::Mouse::getPosition(window).y<=this->bottomRight.get_y());
        if(this->drawFnc)
            this->drawFnc(*this, hover, window);
    }
    [[nodiscard]] vec2i getTopLeft() {return this->topLeft;}
    [[nodiscard]] vec2i getBottomRight() {return this->bottomRight;}
};

void handleMovement(Entity& player) {player.setVelocity(vec2f((float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)),-(float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))).normalize()*=4);}

void handleShooting(std::vector<Entity*>& bullets, long long& last_bullet, Entity& player, sf::RenderWindow& window)
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && std::chrono::system_clock::now().time_since_epoch().count()-last_bullet>=bullet_time_spawn)
    {
        last_bullet=std::chrono::system_clock::now().time_since_epoch().count();
        auto bullet=new Entity(player.getCenter(),2);
        sf::Vector2i mousePos=sf::Mouse::getPosition(window);
        bullet->setVelocity((vec2f((float)mousePos.x,(float)mousePos.y)-player.getCenter()).normalize()*=11);
        bullet->setColor(sf::Color(255,255,0));
        bullets.push_back(bullet);
    }
}

Entity* trySpawnMonster(vec2f center, const float chancePerTick=0.01/5.6, const float radius=80)
{
    if((float)rand()/(float)RAND_MAX<chancePerTick)
    {
        vec2f pos(((float)rand()/(float)RAND_MAX)*(float)WINDOWWIDTH,((float)rand()/(float)RAND_MAX)*(float)WINDOWHEIGHT);
        while((pos-center).lengthSquared()<radius*radius)
            pos=vec2f(((float)rand()/(float)RAND_MAX)*(float)WINDOWWIDTH,((float)rand()/(float)RAND_MAX)*(float)WINDOWHEIGHT);
        auto monstru=new Entity(pos,9);
        monstru->setColor(sf::Color(255,0,0));
        return monstru;
    }
    return nullptr;
}

bool gameLogic(Entity& player, std::vector<Entity*>& monstri, std::vector<Entity*>& bullets)
{
    int i, j;
    for(i=0;i<(int)monstri.size();++i)
    {
        if(Physics::checkIntersection(&player, monstri[i]))
        {
            if(player.takeDamage(5)<=0)
            {
                return true;
            }
        }
        else
        {
            monstri[i]->setVelocity((player.getCenter()-monstri[i]->getCenter()).normalize()*=2);
        }

        for(j=0;j<(int)bullets.size();++j)
        {
            if(Physics::checkIntersection(monstri[i],bullets[j]))
            {
                std::swap(bullets[j], bullets[(int)bullets.size()-1]);
                delete bullets[(int)bullets.size()-1];
                bullets.pop_back();
                --j;
                if(monstri[i]->takeDamage(20)<=0)
                {
                    std::swap(monstri[i], monstri[(int)monstri.size()-1]);
                    delete monstri[(int)monstri.size()-1];
                    monstri.pop_back();
                    --i;
                    j=(int)bullets.size();
                }
            }
        }
    }
    return false;
}

enum MENU{EXITGAME, MAINMENU, GAMEPLAY, PAUSEMENU, MENUCNT};
MENU prevMenu=MAINMENU;
MENU menu=MAINMENU;

void pauseGame() {menu=PAUSEMENU;}
void resumeGame() {menu=GAMEPLAY;}
void goToMainMenu() {menu=MAINMENU;}
void exitGame() {menu=EXITGAME;}

void drawPauseButton(Button& btn, bool hover, sf::RenderWindow& window)
{
    sf::RectangleShape rect;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(191, 0, 0));
    else
        rect.setFillColor(sf::Color(255, 0, 0));

    window.draw(rect);

    dim/=5;

    rect.setPosition(sf::Vector2f((float)(topLeft+dim).get_x(), (float)(topLeft+dim).get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), 3*(float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(191, 191, 191));
    else
        rect.setFillColor(sf::Color(255, 255, 255));

    window.draw(rect);

    rect.setPosition(sf::Vector2f((float)(topLeft+3*dim).get_x(), (float)(topLeft+dim).get_y()));
    window.draw(rect);
}

void drawResumeButton(Button& btn, bool hover, sf::RenderWindow& window)
{
	sf::RectangleShape rect;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(0, 191, 0));
    else
        rect.setFillColor(sf::Color(0, 255, 0));

    window.draw(rect);

    dim/=5;
    sf::CircleShape triangle((float)dim.get_x()*2, 3);

    triangle.setRotation(90);
    triangle.setPosition(sf::Vector2f((float)topLeft.get_x()+(float)(dim*4).get_x(), (float)(topLeft+dim/2).get_y()));
    if(hover)
        triangle.setFillColor(sf::Color(191, 191, 191));
    else
        triangle.setFillColor(sf::Color(255, 255, 255));

    window.draw(triangle);
}

void drawXButton(Button& btn, bool hover, sf::RenderWindow& window)
{
    sf::RectangleShape rect;
    sf::ConvexShape line;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft, aux;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(191, 0, 0));
    else
        rect.setFillColor(sf::Color(255, 0, 0));

    window.draw(rect);

    aux=dim/8;

    line.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    line.setPointCount(6);
    if(hover)
        line.setFillColor(sf::Color(191, 191, 191));
    else
        line.setFillColor(sf::Color(255, 255, 255));

    line.setPoint(0, sf::Vector2f(0, 0));
    line.setPoint(1, sf::Vector2f(0, (float)aux.get_y()));
    line.setPoint(2, sf::Vector2f((float)dim.get_x()-(float)aux.get_x(), (float)dim.get_y()));
    line.setPoint(3, sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    line.setPoint(4, sf::Vector2f((float)dim.get_x(), (float)dim.get_y()-(float)aux.get_y()));
    line.setPoint(5, sf::Vector2f((float)aux.get_x(), 0));

    window.draw(line);

    line.setPoint(0, sf::Vector2f((float)dim.get_x(), 0));
    line.setPoint(1, sf::Vector2f((float)dim.get_x(), (float)aux.get_y()));
    line.setPoint(2, sf::Vector2f((float)aux.get_x(), (float)dim.get_y()));
    line.setPoint(3, sf::Vector2f(0, (float)dim.get_y()));
    line.setPoint(4, sf::Vector2f(0, (float)dim.get_y()-(float)aux.get_y()));
    line.setPoint(5, sf::Vector2f((float)dim.get_x()-(float)aux.get_x(), 0));

    window.draw(line);
}

void render(sf::RenderWindow& window, Entity& player, std::vector<Entity*>& monstri, std::vector<Entity*>& bullets)
{
	int i;
	vec2f pos_bullet;
	float rad;

	player.draw(window);
	for(i=0;i<(int)monstri.size();++i)
		monstri[i]->draw(window);
    for(i=0;i<(int)bullets.size();++i)
    {
        pos_bullet=bullets[i]->getCenter();
        rad=bullets[i]->getRadius();
        if(pos_bullet.get_x()<-rad || pos_bullet.get_x()>(float)WINDOWWIDTH+rad || pos_bullet.get_y()<-rad || pos_bullet.get_y()>(float)WINDOWHEIGHT+rad)
        {
            std::swap(bullets[i], bullets[(int)bullets.size()-1]);
            bullets.pop_back();
        }
        else
            bullets[i]->draw(window);
    }
}

void tick(sf::RenderWindow& window, Entity& player, std::vector<Entity*>& monstri, std::vector<Entity*>& bullets, long long& last_bullet, const float dt)
{
    int i;

	//Event handling:
	handleMovement(player);

	handleShooting(bullets, last_bullet, player, window);

	//Game Logic
	if(Entity* monstru=trySpawnMonster(player.getCenter())) monstri.push_back(monstru);

	if(gameLogic(player, monstri, bullets))
	{
		menu=MAINMENU;
        return;
	}

	//Physics update
	player.tick(dt);
	for(i=0;i<(int)monstri.size();++i)
	{
		monstri[i]->tick(dt);
	}
	for(i=0;i<(int)bullets.size();++i)
	{
		bullets[i]->tick(dt);
	}
}

int main()
{
    WINDOWHEIGHT=600;
    WINDOWWIDTH=600;
    sf::RenderWindow window;
    // NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:30
    window.create(sf::VideoMode({600, 600}), "Shooter", sf::Style::Default);
    window.setVerticalSyncEnabled(false);
    //window.setFramerateLimit(60);

    const float dt=1/32.f;

    Button pauseButton(vec2i(0, 0), vec2i(50, 50), pauseGame, drawPauseButton);
    Button resumeButton(vec2i(200, 200), vec2i(250, 250), resumeGame, drawResumeButton);
    Button playGameButton(vec2i(268, 268), vec2i(332, 332), resumeGame, drawResumeButton);
    Button mainMenuButton(vec2i(350, 200), vec2i(400, 250), goToMainMenu, drawXButton);
    Button exitGameButton(vec2i(550, 0), vec2i(600, 50), exitGame, drawXButton);

    std::vector<Button*> buttons[MENUCNT];
    buttons[GAMEPLAY].push_back(&pauseButton);
    buttons[PAUSEMENU].push_back(&resumeButton);
    buttons[MAINMENU].push_back(&playGameButton);
    buttons[PAUSEMENU].push_back(&mainMenuButton);
    buttons[MAINMENU].push_back(&exitGameButton);

    int i;
    Entity player(vec2f(), 10);
    player.setColor(sf::Color(0, 0, 255));
    long long last_frame=std::chrono::system_clock::now().time_since_epoch().count();
    long long last_bullet=last_frame;
    const long long frame_time=3000000;
    std::vector<Entity*> monstri;
    std::vector<Entity*> bullets;

    srand(time(nullptr));

    player.setCenter(vec2f((float)(WINDOWWIDTH>>1), (float)(WINDOWHEIGHT>>1)));

    while(window.isOpen())
    {
        if(std::chrono::system_clock::now().time_since_epoch().count()-last_frame>frame_time)
        {
            last_frame=std::chrono::system_clock::now().time_since_epoch().count();

            //Non game logic event handling
			sf::Event e=sf::Event();
			while(window.pollEvent(e))
			{
				switch(e.type)
				{
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::Resized:
						//std::cout << "New width: " << window.getSize().x << '\n'
						//		  << "New height: " << window.getSize().y << '\n';
						WINDOWWIDTH=(int)window.getSize().x;
						WINDOWHEIGHT=(int)window.getSize().y;
						break;
					case sf::Event::MouseButtonPressed:
						for(i=0;i<(int)buttons[menu].size();++i)
							buttons[menu][i]->click(vec2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
					default:
						break;
				}
			}

            if(menu==EXITGAME)
            {
                window.close();
                break;
            }

			window.clear();

			if(menu==GAMEPLAY)
			{
				tick(window, player, monstri, bullets, last_bullet, dt);
                if(menu==MAINMENU && prevMenu!=MAINMENU)
                {
                    player.setCenter(vec2f((float)(WINDOWWIDTH>>1), (float)(WINDOWHEIGHT>>1)));
                    for(i=0;i<(int)monstri.size();++i)
                    {
                        delete monstri[i];
                        monstri[i]=nullptr;
                    }
                    monstri.clear();
                    for(i=0;i<(int)bullets.size();++i)
                    {
                        delete bullets[i];
                        bullets[i]=nullptr;
                    }
                    bullets.clear();
                }
				render(window, player, monstri, bullets);
			}

			for(i=0;i<(int)buttons[menu].size();++i)
				buttons[menu][i]->draw(window);

			window.display();

			prevMenu=menu;
        }
    }

    for(i=0;i<(int)monstri.size();++i)
        delete monstri[i];
    for(i=0;i<(int)bullets.size();++i)
        delete bullets[i];

    return 0;
}
