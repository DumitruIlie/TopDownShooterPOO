//Ilie Dumitru
#ifndef PHYSICS_H
#define PHYSICS_H

#include"Entity.h"

class Physics
{
public:
    Physics() = delete;

    static bool checkIntersection(Entity* A, Entity* B) {return (A->getCenter()-B->getCenter()).lengthSquared()<=(A->getRadius()+B->getRadius())*(A->getRadius()+B->getRadius());}
};

#endif//PHYSICS_H