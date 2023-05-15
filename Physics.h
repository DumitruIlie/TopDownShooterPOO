//Ilie Dumitru
#ifndef PHYSICS_H
#define PHYSICS_H

#include"Entity.h"

class Physics
{
public:
    Physics() = delete;

    static bool checkIntersection(Entity* A, Entity* B);
};

#endif//PHYSICS_H