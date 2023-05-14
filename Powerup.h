//Ilie Dumitru
#ifndef POWERUP_H
#define POWERUP_H

#include"Entity.h"

class Entity;

class Powerup
{
protected:
    float value;
    MODIFIABLE modifies;
    bool typeAditive;

public:
    explicit Powerup(float _value=0, MODIFIABLE _modifies=SPEEDFACTOR, bool isAditive=true);

    virtual void affectEntity(Entity* target) const;
};

class SpeedIncreasePowerup : public Powerup
{
public:
    explicit SpeedIncreasePowerup(float _value=0);
};

class DamageIncreasePowerup : public Powerup
{
public:
    explicit DamageIncreasePowerup(float _value=0);
};

class HealthIncreasePowerup : public Powerup
{
public:
    explicit HealthIncreasePowerup(float hp=0);

    void affectEntity(Entity* target) const override;
};

#endif//POWERUP_H