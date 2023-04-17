//Ilie Dumitru
#include"Entity.h"

void Entity::draw(sf::RenderWindow&window) const
{
    sf::CircleShape shape(this->radius);
    shape.setPosition(sf::Vector2f(this->center.get_x()-this->radius, this->center.get_y()-this->radius));
    shape.setFillColor(this->color);
    window.draw(shape);
}