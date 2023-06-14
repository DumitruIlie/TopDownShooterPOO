//Ilie Dumitru
#ifndef OBSERVER_H
#define OBSERVER_H

#include"Achievement.h"
#include<vector>

class Observer
{
private:
    //ach[updateType] e un vector de dimensiune variabila ce stocheaza pointeri la achievementurile ce trebuiesc updatate de acel tip de update
    std::vector<Achievement*> achs[AchUpdateTypeCnt];
    Achievement* aux[Achievement::AchTypeCnt];

    Observer();

    ~Observer();

    void addSubscriber(Achievement* ach, AchUpdateType updtTp);
    
public:
    static Observer& getInstance();

    void update(AchUpdateType updt);
};

#endif//OBSERVER_H