//Ilie Dumitru
#include"Powerup.h"

Powerup::Powerup(const float _value, const MODIFIABLE _modifies, const bool _isAditive) : value(_value), modifies(_modifies), typeAditive(_isAditive) {}

void Powerup::affectEntity(Entity* const target) const
{
    if(this->typeAditive)
        target->modifiables[this->modifies]+=this->value;
    else
        target->modifiables[this->modifies]*=this->value;
}

SpeedIncreasePowerup::SpeedIncreasePowerup(const float _value) : Powerup(_value+1, SPEEDFACTOR, false) {}

DamageIncreasePowerup::DamageIncreasePowerup(const float _value) : Powerup(1+_value, DAMAGEFACTOR, false) {}

HealthIncreasePowerup::HealthIncreasePowerup(const float _value) : Powerup(_value, HEALTH, true) {}

void HealthIncreasePowerup::affectEntity(Entity* const target) const
{
    target->takeDamage(-this->value);
}