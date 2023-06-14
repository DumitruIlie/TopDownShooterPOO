//Ilie Dumitru
#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include<string>
#include<chrono>

enum AchUpdateType{RESPAWN_PLAYER,
                   PINBALL_SCOUT_KILL, BEE_HEAVY_KILL, BULLET_NORMAL_KILL,
                   KILL_SCOUT, SPAWN_SCOUT,
                   UPDATE_TIME,
                   STOP_FIRE, FIRE_NORMAL_BULLET, FIRE_BEE, BEE_HIT,
                   PINBALL_MAX_HITS,
                   ACHIEVEMENT_GET,
                   AchUpdateTypeCnt};

class Achievement
{
private:
    bool achieved;

public:
    static const int AchTypeCnt=9;

    Achievement();

    virtual ~Achievement() = default;

    virtual void update(AchUpdateType updt) = 0;

    virtual std::string getName() = 0;

    virtual std::string getDesc() = 0;

    [[nodiscard]] bool isAchieved() const;

    void achieve();
};

class Ach_Kill_Scouts_Pinballs : public Achievement
{
private:
    int cnt;
    int max;

public:
    Ach_Kill_Scouts_Pinballs();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Kill_Heavies_Bees : public Achievement
{
private:
    int cnt;
    int max;

public:
    Ach_Kill_Heavies_Bees();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Kill_Normals_Bullets : public Achievement
{
private:
    int cnt;
    int max;

public:
    Ach_Kill_Normals_Bullets();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Still_Fighting : public Achievement
{
private:
    bool started;
    std::chrono::duration<long long> init_sec;
    std::chrono::duration<long long> required;

public:
    Ach_Still_Fighting();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Outrunning_Scouts : public Achievement
{
private:
    int cntScoutAlive;
    int required;

public:
    Ach_Outrunning_Scouts();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Apiculture : public Achievement
{
private:
    int cnt;
    int required;

public:
    Ach_Apiculture();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Pinball_Highscore : public Achievement
{
private:
    int cnt;
    int required;

public:
    Ach_Pinball_Highscore();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_I_Am_Heavy : public Achievement
{
private:
    bool isShooting;
    std::chrono::duration<long long> start;
    std::chrono::duration<long long> required;

public:
    Ach_I_Am_Heavy();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

class Ach_Completionist : public Achievement
{
private:
    int cnt;

public:
    Ach_Completionist();

    void update(AchUpdateType updt) override;

    std::string getName() override;

    std::string getDesc() override;
};

#endif//ACHIEVEMENT_H