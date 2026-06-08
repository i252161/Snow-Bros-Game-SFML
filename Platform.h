#pragma once
#include <SFML/Graphics.hpp>

class Platform
{
public:
    Platform();
    Platform(float px, float py, float pw, float ph, sf::Color col);

    void draw(sf::RenderWindow& window);
    sf::FloatRect getrect() const;

    float getX() const;
    float getY() const;
    float getwidth() const;
    float getheight() const;

private:
    float x;
    float y;
    float w;
    float h;
    sf::RectangleShape shape;
    sf::Color color;
};