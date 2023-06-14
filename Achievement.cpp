//Ilie Dumitru
#include"Achievement.h"
#include"Observer.h"

Achievement::Achievement() : achieved(false) {}

bool Achievement::isAchieved() const {return this->achieved;}

void Achievement::achieve() {this->achieved=true; Observer::getInstance().update(ACHIEVEMENT_GET); printf("Achievement get: %s\n%s\n", this->getName().c_str(), this->getDesc().c_str());}

Ach_Kill_Scouts_Pinballs::Ach_Kill_Scouts_Pinballs() : Achievement(), cnt(0), max(10) {}

void Ach_Kill_Scouts_Pinballs::update(const AchUpdateType updt)
{
    if(!this->isAchieved() && updt==PINBALL_SCOUT_KILL)
    {
        if(++this->cnt==this->max)
            achieve();
    }
}

std::string Ach_Kill_Scouts_Pinballs::getName() {return "Don't touch sandwich!";}

std::string Ach_Kill_Scouts_Pinballs::getDesc() {return "Kill "+std::to_string(this->max)+" scouts with pinballs.";}

Ach_Kill_Heavies_Bees::Ach_Kill_Heavies_Bees() : Achievement(), cnt(0), max(10) {}

void Ach_Kill_Heavies_Bees::update(const AchUpdateType updt)
{
    if(!this->isAchieved() && updt==BEE_HEAVY_KILL)
    {
        if(++this->cnt==this->max)
            this->achieve();
    }
}

std::string Ach_Kill_Heavies_Bees::getName() {return "The soviet onion";}

std::string Ach_Kill_Heavies_Bees::getDesc() {return "Kill "+std::to_string(this->max)+" heavies with bees.";}

Ach_Kill_Normals_Bullets::Ach_Kill_Normals_Bullets() : Achievement(), cnt(0), max(100) {}

void Ach_Kill_Normals_Bullets::update(const AchUpdateType updt)
{
    if(!this->isAchieved() && updt==BULLET_NORMAL_KILL)
    {
        if(++this->cnt==this->max)
            this->achieve();
    }
}

std::string Ach_Kill_Normals_Bullets::getName() {return "Masacre";}

std::string Ach_Kill_Normals_Bullets::getDesc() {return "Kill "+std::to_string(this->max)+" normal enemies with normal bullets.";}

Ach_Still_Fighting::Ach_Still_Fighting() : Achievement(), started(false), init_sec(), required(std::chrono::minutes(5)) {}

void Ach_Still_Fighting::update(const AchUpdateType updt)
{
    if(!this->isAchieved())
    {
        if(updt==RESPAWN_PLAYER)
        {
            this->started=true;
            this->init_sec=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
        }
        else if(updt==UPDATE_TIME)
        {
            if(this->started && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())-this->init_sec>=this->required)
            {
                this->achieve();
            }
        }
    }
}

std::string Ach_Still_Fighting::getName() {return "Still fighting";}

std::string Ach_Still_Fighting::getDesc() {return "Play for 5 minutes without dying";}

Ach_Outrunning_Scouts::Ach_Outrunning_Scouts() : Achievement(), cntScoutAlive(0), required(5) {}

void Ach_Outrunning_Scouts::update(const AchUpdateType updt)
{
    if(!this->isAchieved())
    {
        if(updt==KILL_SCOUT)
            --this->cntScoutAlive;
        else if(updt==SPAWN_SCOUT)
        {
            if(++this->cntScoutAlive==this->required)
                this->achieve();
        }
        else if(updt==RESPAWN_PLAYER)
            this->cntScoutAlive=0;
    }
}

std::string Ach_Outrunning_Scouts::getName() {return "Outran you damn scot";}

std::string Ach_Outrunning_Scouts::getDesc() {return "Outrun "+std::to_string(this->required)+" scouts";}

Ach_Apiculture::Ach_Apiculture() : Achievement(), cnt(0), required(35) {}

void Ach_Apiculture::update(const AchUpdateType updt)
{
    if(!this->isAchieved())
    {
        if(updt==RESPAWN_PLAYER)
            this->cnt=0;
        else if(updt==FIRE_BEE)
        {
            if(++this->cnt==this->required)
                this->achieve();
        }
        else if(updt==BEE_HIT)
            --this->cnt;
    }
}

std::string Ach_Apiculture::getName() {return "It ain't much";}

std::string Ach_Apiculture::getDesc() {return "Have "+std::to_string(this->required)+" bees on the screen.";}

Ach_Pinball_Highscore::Ach_Pinball_Highscore() : Achievement(), cnt(0), required(3) {}

void Ach_Pinball_Highscore::update(const AchUpdateType updt)
{
    if(!this->isAchieved() && updt==PINBALL_MAX_HITS)
    {
        if(++this->cnt==this->required)
            this->achieve();
    }
}

std::string Ach_Pinball_Highscore::getName() {return "Professionals have standards";}

std::string Ach_Pinball_Highscore::getDesc() {return "Hit the maximum number of enemies with a pinball "+std::to_string(this->required)+" times.";}

Ach_I_Am_Heavy::Ach_I_Am_Heavy() : Achievement(), isShooting(false), start(), required(std::chrono::seconds(12)) {}

void Ach_I_Am_Heavy::update(const AchUpdateType updt)
{
    if(!this->isAchieved())
    {
        if(updt==RESPAWN_PLAYER || updt==STOP_FIRE)
            this->isShooting=false;
        else if(updt==UPDATE_TIME)
        {
            if(this->isShooting && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())-this->start>=this->required)
                this->achieve();
        }
        else if(updt==FIRE_NORMAL_BULLET)
        {
            if(!this->isShooting)
            {
                this->start=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
                this->isShooting=true;
            }
        }
    }
}

std::string Ach_I_Am_Heavy::getName() {return "She wheighs 150 kg";}

std::string Ach_I_Am_Heavy::getDesc() {return "And fires $200 custom tool cardriges at 10,000 rounds per minute. It costs $400,000 to fire this weapon for 12 seconds.";}

Ach_Completionist::Ach_Completionist() : Achievement(), cnt(0) {}

void Ach_Completionist::update(AchUpdateType updt)
{
    if(!this->isAchieved())
    {
        if(updt==ACHIEVEMENT_GET)
        {
            if(++this->cnt==Achievement::AchTypeCnt-1)
                this->achieve();
        }
    }
}

std::string Ach_Completionist::getName() {return "You completed all achievements!";}

std::string Ach_Completionist::getDesc() {return "Nice job!";}