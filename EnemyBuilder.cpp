//Ilie Dumitru
#include"EnemyBuilder.h"

EnemyBuilder::EnemyBuilder(Game* _game) : beingBuilt(new Humanoid(_game)), game(_game) {}

EnemyBuilder::EnemyBuilder(const EnemyBuilder& other) : game(other.game) {if(other.beingBuilt) this->beingBuilt=dynamic_cast<Humanoid*>(other.beingBuilt->clone()); else this->beingBuilt=nullptr;}

EnemyBuilder::EnemyBuilder(EnemyBuilder&& other) noexcept : beingBuilt(other.beingBuilt), game(other.game) {other.beingBuilt=nullptr;}

EnemyBuilder& EnemyBuilder::operator=(const EnemyBuilder& other)
{
    if(this==&other)
        return *this;
    this->game=other.game;
    delete this->beingBuilt;
    if(!other.beingBuilt)
        this->beingBuilt=nullptr;
    else
        this->beingBuilt=dynamic_cast<Humanoid*>(other.beingBuilt->clone());
    return *this;
}

EnemyBuilder& EnemyBuilder::operator=(EnemyBuilder&& other) noexcept
{
    this->game=other.game;
    delete this->beingBuilt;
    this->beingBuilt=other.beingBuilt;
    other.beingBuilt=nullptr;
    return *this;
}

EnemyBuilder::~EnemyBuilder() {delete this->beingBuilt;}

void EnemyBuilder::checkMissing() {if(!this->beingBuilt) this->beingBuilt=new Humanoid(this->game);}

EnemyBuilder& EnemyBuilder::setPos(const vec2f pos) {this->checkMissing(); this->beingBuilt->setCenter(pos); return *this;}

EnemyBuilder& EnemyBuilder::setSize(const float radius) {this->checkMissing(); this->beingBuilt->radius=radius; return *this;}

//EnemyBuilder& EnemyBuilder::setInitVelocity(const vec2f vel) {this->checkMissing(); this->beingBuilt->setVelocity(vel); return *this;}

EnemyBuilder& EnemyBuilder::setMaxHealth(const float max_hp, const bool overhealable) {this->checkMissing(); this->beingBuilt->MAX_HP=max_hp*(1+0.5f*(float)overhealable); this->beingBuilt->overhealable=overhealable; this->beingBuilt->hp=this->beingBuilt->MAX_HP; return *this;}

EnemyBuilder& EnemyBuilder::setColor(const sf::Color col) {this->checkMissing(); this->beingBuilt->setColor(col); return *this;}

//EnemyBuilder& EnemyBuilder::setGame(Game* _game) {this->game=_game; this->checkMissing(); this->beingBuilt->game=_game; return *this;}

EnemyBuilder& EnemyBuilder::applyPowerup(const Powerup& powerup) {this->checkMissing(); powerup.affectEntity(this->beingBuilt); return *this;}

Humanoid* EnemyBuilder::spawn() {this->checkMissing(); Humanoid* enemy=this->beingBuilt; this->beingBuilt=nullptr; return enemy;}