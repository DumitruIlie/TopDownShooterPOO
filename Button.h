//Ilie Dumitru
#ifndef BUTTON_H
#define BUTTON_H

#include<SFML/Graphics.hpp>
#include"vec2.h"

class Button
{
private:
    void (*onClick)();
    void (*drawFnc)(Button&, bool, sf::RenderWindow&);

protected:
    vec2i topLeft, bottomRight;

public:
    Button() : onClick(nullptr), drawFnc(nullptr), topLeft(0, 0), bottomRight(0, 0) {}
    Button(vec2i _topLeft, vec2i _bottomRight) : onClick(nullptr), drawFnc(nullptr), topLeft(_topLeft), bottomRight(_bottomRight) {}
    Button(vec2i _topLeft, vec2i _bottomRight, void (*_onClick)(), void (*_drawFnc)(Button&, bool, sf::RenderWindow&)) : onClick(_onClick), drawFnc(_drawFnc), topLeft(_topLeft), bottomRight(_bottomRight) {}
    Button(const Button& other) = default;
    ~Button() = default;
    Button& operator=(const Button& other) = default;

    void click(vec2i mousePos);
    void draw(sf::RenderWindow& window);
    [[nodiscard]] vec2i getTopLeft() {return this->topLeft;}
    [[nodiscard]] vec2i getBottomRight() {return this->bottomRight;}
};

#endif//BUTTON_H