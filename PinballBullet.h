//Ilie Dumitru
#ifndef PINBALLBULLET_H
#define PINBALLBULLET_H

#include"Bullet.h"

class PinballBullet : public Bullet
{
private:
    int state;
    static const int MAXHITS=5;

    PinballBullet(const PinballBullet& other);

public:
    PinballBullet(float _damage, Game* _game, vec2f center=vec2f(), float radius=4);
    ~PinballBullet() override;
};

#endif//PINBALLBULLET_H