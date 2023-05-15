//Ilie Dumitru
#include"Button.h"

Button::Button() : onClick(nullptr), drawFnc(nullptr), topLeft(0, 0), bottomRight(0, 0) {}

Button::Button(const vec2i _topLeft, const vec2i _bottomRight) : onClick(nullptr), drawFnc(nullptr), topLeft(_topLeft), bottomRight(_bottomRight) {}

Button::Button(const vec2i _topLeft, const vec2i _bottomRight, void (*_onClick)(), void (*_drawFnc)(const Button&, bool, sf::RenderWindow&)) : onClick(_onClick), drawFnc(_drawFnc), topLeft(_topLeft), bottomRight(_bottomRight) {}

vec2i Button::getTopLeft() const {return this->topLeft;}

vec2i Button::getBottomRight() const {return this->bottomRight;}

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

void Button::click(const vec2i mousePos) const
{
    if(this->topLeft.get_x()<=mousePos.get_x() &&
       mousePos.get_x()<=this->bottomRight.get_x() &&
       this->topLeft.get_y()<=mousePos.get_y() &&
       mousePos.get_y()<=this->bottomRight.get_y() &&
       this->onClick
      )
        this->onClick();
}