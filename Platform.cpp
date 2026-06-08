#include "Platform.h"

Platform::Platform()
{
    x = 0;
    y = 0;
    w = 100;
    h = 16;
    color = sf::Color(100, 70, 40);
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(w, h));
    shape.setFillColor(color);
}

Platform::Platform(float px, float py, float pw, float ph, sf::Color col)
{
    x = px;
    y = py;
    w = pw;
    h = ph;
    color = col;
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(w, h));
    shape.setFillColor(color);
}

void Platform::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Platform::getrect() const
{
    return sf::FloatRect(x, y, w, h);
}

float Platform::getX() const
{
    return x;
}
float Platform::getY() const
{
    return y;
}
float Platform::getwidth() const
{
    return w;
}
float Platform::getheight() const
{
    return h;
}