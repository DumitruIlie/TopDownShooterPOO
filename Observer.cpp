//Ilie Dumitru
#include"Observer.h"

Observer& Observer::getInstance()
{
    static Observer obs;
    return obs;
}

Observer::Observer() : achs(), aux()
{
    this->aux[0]=new Ach_Kill_Scouts_Pinballs();
    this->aux[1]=new Ach_Kill_Heavies_Bees();
    this->aux[2]=new Ach_Kill_Normals_Bullets();
    this->aux[3]=new Ach_Still_Fighting();
    this->aux[4]=new Ach_Outrunning_Scouts();
    this->aux[5]=new Ach_Apiculture();
    this->aux[6]=new Ach_Pinball_Highscore();
    this->aux[7]=new Ach_I_Am_Heavy();
    this->aux[8]=new Ach_Completionist();

    this->addSubscriber(this->aux[0], PINBALL_SCOUT_KILL);

    this->addSubscriber(this->aux[1], BEE_HEAVY_KILL);

    this->addSubscriber(this->aux[2], BULLET_NORMAL_KILL);

    this->addSubscriber(this->aux[3], RESPAWN_PLAYER);
    this->addSubscriber(this->aux[3], UPDATE_TIME);

    this->addSubscriber(this->aux[4], SPAWN_SCOUT);
    this->addSubscriber(this->aux[4], KILL_SCOUT);
    this->addSubscriber(this->aux[4], RESPAWN_PLAYER);

    this->addSubscriber(this->aux[5], RESPAWN_PLAYER);
    this->addSubscriber(this->aux[5], FIRE_BEE);
    this->addSubscriber(this->aux[5], BEE_HIT);

    this->addSubscriber(this->aux[6], PINBALL_MAX_HITS);

    this->addSubscriber(this->aux[7], RESPAWN_PLAYER);
    this->addSubscriber(this->aux[7], STOP_FIRE);
    this->addSubscriber(this->aux[7], UPDATE_TIME);
    this->addSubscriber(this->aux[7], FIRE_NORMAL_BULLET);

    this->addSubscriber(this->aux[8], ACHIEVEMENT_GET);
}

Observer::~Observer()
{
    int i;

    for(i=0;i<Achievement::AchTypeCnt;++i)
        delete this->aux[i];
}

void Observer::update(const AchUpdateType updt)
{
    int i;

    for(i=0;i<(int)this->achs[updt].size();++i)
        this->achs[updt][i]->update(updt);
}

void Observer::addSubscriber(Achievement* const ach, const AchUpdateType updtTp)
{
    int i;

    for(i=0;i<(int)this->achs[updtTp].size();++i)
        if(this->achs[updtTp][i]==ach)
            i=(int)this->achs[updtTp].size();

    if(i==(int)this->achs[updtTp].size())
        this->achs[updtTp].push_back(ach);
}