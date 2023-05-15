//Ilie Dumitru
#ifndef BUTTON_H
#define BUTTON_H

#include<SFML/Graphics.hpp>
#include"vec2.h"

class Button
{
private:
    void (*onClick)();
    void (*drawFnc)(const Button&, bool, sf::RenderWindow&);

protected:
    vec2i topLeft, bottomRight;

public:
    Button();
    Button(vec2i _topLeft, vec2i _bottomRight);
    Button(vec2i _topLeft, vec2i _bottomRight, void (*_onClick)(), void (*_drawFnc)(const Button&, bool, sf::RenderWindow&));
    Button(const Button& other) = default;
    Button(Button&& other) = default;
    ~Button() = default;
    Button& operator=(const Button& other) = default;
    Button& operator=(Button&& other) = default;

    void click(vec2i mousePos) const;
    void draw(sf::RenderWindow& window);
    [[nodiscard]] vec2i getTopLeft() const;
    [[nodiscard]] vec2i getBottomRight() const;
};

#endif//BUTTON_H