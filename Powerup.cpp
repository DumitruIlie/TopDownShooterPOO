//Ilie Dumitru
#include"Powerup.h"

Powerup::Powerup(float _value, MODIFIABLE _modifies, bool _isAditive) : value(_value), modifies(_modifies), typeAditive(_isAditive) {}

void Powerup::affectEntity(Entity* target) const
{
    if(this->typeAditive)
        target->modifiables[this->modifies]+=this->value;
    else
        target->modifiables[this->modifies]*=this->value;
}

SpeedIncreasePowerup::SpeedIncreasePowerup(float _value) : Powerup(_value+1, SPEEDFACTOR, false) {}

DamageIncreasePowerup::DamageIncreasePowerup(float _value) : Powerup(1+_value, DAMAGEFACTOR, false) {}

HealthIncreasePowerup::HealthIncreasePowerup(float _value) : Powerup(_value, HEALTH, true) {}

void HealthIncreasePowerup::affectEntity(Entity* target) const
{
    target->takeDamage(-this->value);
}