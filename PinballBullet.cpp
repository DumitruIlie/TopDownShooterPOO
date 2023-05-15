//Ilie Dumitru
#include"PinballBullet.h"
#include"AppException.h"

PinballBullet::PinballBullet(const float _damage, Game* const _game, const vec2f center, const float radius) : Bullet(_damage, _game, center, radius), state(0) {}

PinballBullet::PinballBullet(const PinballBullet& other) : Bullet(other), state(other.state+1)
{
    if(this->state==MAXHITS)
        throw MaxPinballHits();
    vec2f vel=(-other.getVelocity()+vec2f((float)rand()/(float)RAND_MAX*(float)this->state/(float)(MAXHITS+1), (float)rand()/(float)RAND_MAX*(float)this->state/(float)(MAXHITS+1))).normalize()*(float)this->state;
    this->setCenter(this->getCenter()+vel*4);
    this->setVelocity(vel);
}

PinballBullet::~PinballBullet() {try {Entity* b=new PinballBullet(*this); this->game->addBullet(b); }catch(MaxPinballHits& m) {}}

void PinballBullet::safeDestr() {this->state=MAXHITS-1;}