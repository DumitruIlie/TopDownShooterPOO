//Ilie Dumitru
#include"Button.h"

void Button::draw(sf::RenderWindow& window)
{
    bool hover=(sf::Mouse::getPosition(window).x>=this->topLeft.get_x() &&
                sf::Mouse::getPosition(window).x<=this->bottomRight.get_x() &&
                sf::Mouse::getPosition(window).y>=this->topLeft.get_y() &&
                sf::Mouse::getPosition(window).y<=this->bottomRight.get_y()
               );
    if(this->drawFnc)
        this->drawFnc(*this, hover, window);
}

void Button::click(vec2i mousePos)
{
    if(this->topLeft.get_x()<=mousePos.get_x() &&
       mousePos.get_x()<=this->bottomRight.get_x() &&
       this->topLeft.get_y()<=mousePos.get_y() &&
       mousePos.get_y()<=this->bottomRight.get_y() &&
       this->onClick
      )
        this->onClick();
}