//Ilie Dumitru
#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<exception>
#include<cmath>
#include<chrono>
#include<cstdlib>

int WINDOWWIDTH, WINDOWHEIGHT;

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
    //void set_x(const T& x) {this->x=x;}
    //void set_y(const T& y) {this->y=y;}

    [[nodiscard]] T lengthSquared() const {return this->x*this->x+this->y*this->y;}
    [[nodiscard]] T length() const {return sqrt(this->x*this->x+this->y*this->y);}
    vec2<T>& normalize() {return *this/=this->length();}
    //[[nodiscard]] vec2<T> normalized() const {return *this/this->length();}
};
//typedef vec2<int> vec2i;
typedef vec2<float>vec2f;

template<class T>
class vector
{
private:
    T* v;
    int size;
    int capacity;

public:
    vector() : v(nullptr), size(0), capacity(0) {}
    vector(int size, const T& defaultValue) : v(0), size(0), capacity(0)
    {
        if(size>0)
        {
            int i;

            this->size=size;
            this->capacity=size;
            this->v=new T[this->capacity];
            for(i=0;i<this->capacity;++i)
                this->v[i]=defaultValue;
        }
    }
    vector(const T* v, int size) : v(0), size(0), capacity(0)
    {
        if(v && size>0)
        {
            int i;
            this->v=new T[this->capacity=this->size=size];
            for(i=0;i<this->capacity;++i)
                this->v[i]=v[i];
        }
    }
    vector(const vector& other) : v(0), size(other.size), capacity(other.capacity)
    {
        if(other.v)
        {
            int i;
            this->v=new T[this->capacity];
            for(i=0;i<this->size;++i)
                this->v[i]=other.v[i];
        }
    }
    virtual ~vector() {if(this->v) delete[] this->v;}

    vector& operator=(const vector& other)
    {
        if(this==&other)
            return *this;
        if(other.v)
        {
            T* aux=new T[other.capacity];
            int i;
            for(i=0;i<other.size;++i)
                aux[i]=other.v[i];
            if(this->v)
                delete[] this->v;
            this->v=aux;
            this->size=other.size;
            this->capacity=other.capacity;
        }
        else
        {
            if(this->v)
            {
                delete[] this->v;
                this->v=0;
                this->capacity=this->size=0;
            }
        }
        return *this;
    }

    vector& operator+=(const T& item)
    {
        this->push_back(std::move(item));
        return *this;
    }

    void push_back(const T& val)
    {
        if(!this->v)
        {
            this->v=new T[this->size=this->capacity=1];
            this->v[0]=std::move(val);
        }
        else
        {
            if(this->size==this->capacity)
            {
                T* aux=new T[this->capacity<<=1];
                int i;
                for(i=0;i<this->size;++i)
                    aux[i]=this->v[i];
                delete[] this->v;
                this->v=aux;
            }
            this->v[this->size++]=std::move(val);
        }
    }
    void pop_back()
    {
        if(this->v)
        {
            if(this->size)
            {
                if(this->size==1)
                {
                    delete[] this->v;
                    this->v=nullptr;
                    this->capacity=this->size=0;
                }
                else if(--this->size<=(this->capacity>>2))
                {
                    int i;
                    T* new_v=new T[this->capacity>>=1];
                    for(i=0;i<this->size;++i)
                        new_v[i]=this->v[i];
                    delete[] this->v;
                    this->v=new_v;
                }
            }
        }
    }

    [[nodiscard]] int getSize() const {return this->size;}
    T& operator[](int idx) {if(this->v && idx<this->size && idx>-1) return this->v[idx]; throw std::exception();}
    void swap(int i, int j) {if(i!=j && i<this->size && i>-1 && j<this->size && j>-1) {T aux(this->v[i]); this->v[i]=this->v[j]; this->v[j]=aux;}}

    friend std::ostream& operator<<(std::ostream& out, const vector<T>& vect)
    {
        int i;
        if(!vect.v)
            return out<<"[]";
        out<<'['<<*vect.v;
        for(i=1;i<vect.size;++i)
            out<<", "<<vect.v[i];
        return out<<']';
    }
    friend std::istream& operator>>(std::istream& in, vector<T>& vect)
    {
        int sz=0;
        T* aux_p;
        vector<T> aux;
        in>>sz;
        if(sz>0)
        {
            int i;
            aux.v=new T[aux.size=aux.capacity=sz];
            for(i=0;i<sz;++i)
                in>>aux.v[i];
            aux_p=aux.v;
            aux.v=vect.v;
            vect.v=aux_p;
            vect.size=vect.capacity=sz;
        }
        return in;
    }

    void swap(vector& other)
    {
        int aux;
        T* aux_v;

        aux=this->capacity;
        this->capacity=other.capacity;
        other.capacity=aux;

        aux=this->size;
        this->size=other.size;
        other.size=aux;

        aux_v=this->v;
        this->v=other.v;
        other.v=aux_v;
    }
};

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
    //void setCenter(const vec2f newCenter) {center=newCenter;}

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
private:


    /*static bool getIntersection(Entity* A, Entity* B, vec2f& normal, float& penDepth)
    {
        if(!checkIntersection(A, B))
            return 0;

        normal=A->getCenter()-B->getCenter();
        float len=normal.length();
        penDepth=len-A->getRadius()-B->getRadius();
        normal/=len;
        return 1;
    }*/

protected:

public:
    Physics() = delete;

    static bool checkIntersection(Entity* A, Entity* B) {return (A->getCenter()-B->getCenter()).lengthSquared()<=(A->getRadius()+B->getRadius())*(A->getRadius()+B->getRadius());}

    /*static void solve(Entity* A, Entity* B, int dt)
    {
        float penDepth;
        vec2f delta;
        getIntersection(A, B, delta, penDepth);
        float velNormal=delta*(A->getVelocity()-B->getVelocity());
        float j=-1/(velNormal*(A->getWeight_1()+A->getWeight_1()));

        vec2f impulse=j*delta;
        A->setVelocity(A->getVelocity()-impulse*A->getWeight_1()*dt);
        B->setVelocity(B->getVelocity()+impulse*B->getWeight_1()*dt);
    }*/
};
/*
class GameEntity;

class Scene
{
private:
	vector<GameEntity*> entitati;
	GameEntity* player;

protected:

public:
	Scene() : entitati(0, (GameEntity*)0) {}
	Scene(const vector<GameEntity*>& entitati);
	~Scene()
	{
		int i;
		for(i=0;i<this->entitati.getSize();++i)
			delete this->entitati[i];
	}

	void tick(const float dt);

	void spawn(const GameEntity& entity);
	void spawn(GameEntity* const entity) {this->entitati.push_back(entity);}

	void draw(sf::RenderWindow* window);

	void setPlayer(GameEntity* const newPlayer) {this->player=newPlayer;}
};

enum GameEntityType{PlayerGameEntity, MonsterGameEntity, BulletGameEntity, GameEntityTypeCnt};

class GameEntity
{
private:

protected:
	Entity body;
	float health;
	GameEntityType type;

public:
	GameEntity(GameEntityType type=PlayerGameEntity) : health(100), type(type) {}
	GameEntity(const Entity& body, GameEntityType type=PlayerGameEntity) : body(body), health(100), type(type) {}
	virtual ~GameEntity() {}

	virtual void draw(sf::RenderWindow* window) const = 0;

	vec2f get_pos() const {return this->body.getCenter();}
	void set_pos(vec2f newPos) {this->body.setCenter(newPos);}

	virtual void tick(const float dt) = 0;
	virtual GameEntity* makeCopy() const = 0;
	Entity* getBody() {return &this->body;}

	const GameEntityType getType() const {return this->type;}

	virtual void takeDamage(GameEntity* bullet);
};

void Scene::spawn(const GameEntity& entity) {this->entitati.push_back(entity.makeCopy());}
void Scene::draw(sf::RenderWindow* window) {int i; for(i=0;i<this->entitati.getSize();++i) this->entitati[i]->draw(window);}
Scene::Scene(const vector<GameEntity*>& entitati, int playerIndex) : entitati(entitati) {player=this->entitati[playerIndex];}
void Scene::tick(const float dt)
{
	int i, j;
	for(i=0;i<this->entitati.getSize();++i)
	{
		for(j=i+1;j<this->entitati.getSize();++j)
		{
			if(this->entitati[i]->getType()==MonsterGameEntity && this->entitati[j]->getType()==BulletGameEntity)
			{
				if(Physics::checkIntersection(this->entitati[i]->getBody(), this->entitati[j]->getBody()))
				{
					this->entitati[j]->takeDamage(this->entitati[i]);
				}
			}
			else
			{
				Physics::solve(this->entitati[i]->getBody(), this->entitati[j]->getBody(), dt);
			}
		}
	}
	for(i=0;i<this->entitati.getSize();++i)
	{
		this->entitati[i]->getBody()->tick(dt);
	}
}

class Monster : public GameEntity
{
private:

protected:
	Scene* scene;
	GameEntity* target;

public:
	Monster(vec2f pos=vec2f()) : GameEntity(pos) {}
	virtual ~Monster() {}

	virtual void draw(sf::RenderWindow* window) const
	{
		//sf::setcolor(red);
		//window->fillCircle(this->body.getCenter(), this->body.getRadius());
	}

	friend std::ostream& operator<<(std::ostream& out, const Monster& m) {return out<<m.body;}

	void setTarget(GameEntity* target) {this->target=target;}

	virtual void tick(const float dt) override
	{
		if(this->target)
		{
			this->body.setVelocity((this->target->getBody()->getCenter()-this->body.getCenter()).normalized());
		}
		else
		{
			this->body.setVelocity(vec2f());
		}
		this->body.tick(dt);
	}

	virtual GameEntity* makeCopy() const {return new Monster(*this);}
};

class Player : public GameEntity
{
private:
	Monster* enemy;
	Scene* scene;

protected:

public:
	Player() = default;
	virtual ~Player();

	virtual void draw(sf::RenderWindow* window) const
	{
		//sf::setcolor(red);
		//window->fillCircle(this->body.getCenter(), this->body.getRadius());
	}

	friend std::ostream& operator<<(std::ostream& out, const Player& p)
	{
		return out<<p.body;
	}

	void attack(Monster* enemy)
	{
		this->enemy=enemy;
	}

	void shoot() const
	{

	}

	virtual void tick()
	{

	}

	void setScene(Scene* const scene) {this->scene=scene;}

	virtual GameEntity* makeCopy() const override {return new Player(*this);}
};

class Bullet : public GameEntity
{
private:

protected:
	float damage;

public:
	Bullet(vec2f pos=vec2f()) : GameEntity(pos), damage(1) {}
	~Bullet() = default;

	virtual void tick(const float dt)
	{
		if(this->body.center.get_x()<-WORLDWIDTH || this->body.center.get_y()<-WORLDHEIGHT || this->body.center.get_x()>WORLDWIDTH || this->body.getCenter().get_y()>WORLDHEIGHT)
		{

		}
	}

	virtual void draw(sf::RenderWindow* window) const
	{
		//sf::setcolor(red);
		//window->fillCircle(this->body.getCenter(), this->body.getRadius());
	}

	virtual float getDamage() const {return this->damage;}
};

void GameEntity::takeDamage(GameEntity* _bullet)
{
	Bullet* bullet=(Bullet*)_bullet;
	this->health-=bullet->getDamage();
}*/

int main()
{
    sf::RenderWindow window;
    // NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:30
    window.create(sf::VideoMode({600, 600}), "Shooter", sf::Style::Default);
    window.setVerticalSyncEnabled(false);
    //window.setFramerateLimit(60);

    WINDOWHEIGHT=600;
    WINDOWWIDTH=600;
    const float dt=1/32.f;

    int i, j;
    Entity player(vec2f(), 10);
    player.setColor(sf::Color(0, 0, 255));
    long long last_frame=std::chrono::system_clock::now().time_since_epoch().count();
    long long last_bullet=last_frame;
    const long long bullet_time_spawn=200000000LL;
    const long long frame_time=3000000;
    vector<Entity*> monstri;
    vector<Entity*> bullets;
    bool lost=false;

    //srand(time(nullptr));

    while(window.isOpen())
    {
        if(std::chrono::system_clock::now().time_since_epoch().count()-last_frame>frame_time)
        {
            last_frame=std::chrono::system_clock::now().time_since_epoch().count();

            //Event handling:
            vec2f movement((float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)),-(float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)-sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)));
            player.setVelocity(movement.normalize()*=4);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && std::chrono::system_clock::now().time_since_epoch().count()-last_bullet>=bullet_time_spawn)
            {
                last_bullet=std::chrono::system_clock::now().time_since_epoch().count();
                auto bullet=new Entity(player.getCenter(),2);
                sf::Vector2i mousePos=sf::Mouse::getPosition(window);
                bullet->setVelocity((vec2f((float)mousePos.x,(float)mousePos.y)-player.getCenter()).normalize()*=11);
                bullet->setColor(sf::Color(255,255,0));
                bullets+=bullet;
            }

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
                    default:
                        break;
                }
            }

            //Game Logic
            if((float)rand()/(float)RAND_MAX<0.01/4/*vrem sa se spawneze un monstru la aprox 10 secunde, sunt 60 de frame-uri pe secunda => 10%/60 = 1/600*/)
            {
                vec2f pos(((float)rand()/(float)RAND_MAX)*(float)WINDOWWIDTH,((float)rand()/(float)RAND_MAX)*(float)WINDOWHEIGHT);
                while((pos-player.getCenter()).lengthSquared()<1600)
                    pos=vec2f(((float)rand()/(float)RAND_MAX)*(float)WINDOWWIDTH,((float)rand()/(float)RAND_MAX)*(float)WINDOWHEIGHT);
                auto monstru=new Entity(pos,9);
                monstru->setColor(sf::Color(255,0,0));
                monstri+=monstru;
            }

            for(i=0;i<monstri.getSize();++i)
            {
                if(Physics::checkIntersection(&player,monstri[i]))
                {
                    if(player.takeDamage(5)<=0){
                        //Ai pierdut
                        lost=true;
                        break;
                    }
                }
                else
                {
                    monstri[i]->setVelocity((player.getCenter()-monstri[i]->getCenter()).normalize()*=2);
                }

                for(j=0;j<bullets.getSize();++j)
                {
                    if(Physics::checkIntersection(monstri[i],bullets[j]))
                    {
                        bullets.swap(j,bullets.getSize()-1);
                        delete bullets[bullets.getSize()-1];
                        bullets.pop_back();
                        --j;
                        if(monstri[i]->takeDamage(20)<=0)
                        {
                            monstri.swap(i,monstri.getSize()-1);
                            delete monstri[monstri.getSize()-1];
                            monstri.pop_back();
                            --i;
                            j=bullets.getSize();
                        }
                    }
                }
            }
            if(lost)
            {
                window.close();
                break;
            }

            //Physics update
            player.tick(dt);
            for(i=0;i<monstri.getSize();++i)
            {
                monstri[i]->tick(dt);
            }
            for(i=0;i<bullets.getSize();++i)
            {
                bullets[i]->tick(dt);
            }

            //Rendering
            window.clear();

            player.draw(window);
            for(i=0;i<monstri.getSize();++i)
            {
                monstri[i]->draw(window);
            }
            for(i=0;i<bullets.getSize();++i)
            {
                vec2f pos_bullet=bullets[i]->getCenter();
                float rad=bullets[i]->getRadius();
                if(pos_bullet.get_x()<-rad || pos_bullet.get_x()>WINDOWWIDTH+rad || pos_bullet.get_y()<-rad || pos_bullet.get_y()>WINDOWHEIGHT+rad)
                {
                    bullets.swap(i, bullets.getSize()-1);
                    bullets.pop_back();
                }
                else
                    bullets[i]->draw(window);
            }

            window.display();
        }
    }

    for(i=0;i<monstri.getSize();++i)
        delete monstri[i];
    for(i=0;i<bullets.getSize();++i)
        delete bullets[i];

    return 0;
}
