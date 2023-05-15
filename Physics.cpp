//Ilie Dumitru
#include"Physics.h"

bool Physics::checkIntersection(Entity* A, Entity* B) {return (A->getCenter()-B->getCenter()).lengthSquared()<=(A->getRadius()+B->getRadius())*(A->getRadius()+B->getRadius());}