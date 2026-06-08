#include "PowerUp.h"
#include "Constants.h"
#include <cmath>

Powerup::Powerup()
{
    width = 24;
    height = 24;
    alive = false;
    collected = false;
    lifetime = 0;
    boboffset = 0;
    bobtimer = 0;
    type = Poweruptype::Gem;

    shape.setRadius(12.f);
    shape.setOutlineThickness(2.f);
}

void Powerup::spawn(float px, float py, Poweruptype t)
{
    x = px;
    y = py;
    type = t;
    alive = true;
    collected = false;
    lifetime = 0;
    bobtimer = 0;

    shape.setFillColor(getcolorForType());
    shape.setOutlineColor(sf::Color::White);
    updatehitbox();
}

void Powerup::update(float dt)
{
    if (!alive || collected)
        return;

    float maxlife;
    if (type == Poweruptype::Gem)
        maxlife = COLLECTABLE_LIFETIME;
    else
        maxlife = POWERUP_LIFETIME; lifetime = lifetime + dt;

    if (lifetime > maxlife)
    {
        alive = false;
        return;
    }

    bobtimer = bobtimer + dt * 3.0f;
    boboffset = std::sin(bobtimer) * 4.0f;

    updatehitbox();
}

void Powerup::draw(sf::RenderWindow& window)
{
    if (!alive || collected)
        return;

    shape.setPosition(x, y + boboffset);
    window.draw(shape);

    drawhitbox(window, sf::Color::Cyan);
}

Poweruptype Powerup::gettype() const
{
    return type;
}
bool Powerup::iscollected() const
{
    return collected;
}

void Powerup::collect()
{
    collected = true;
    alive = false;
}

sf::Color Powerup::getcolorForType() const
{
    switch (type)
    {
    case Poweruptype::speedboost:
        return sf::Color(255, 200, 0);
    case Poweruptype::snowballpower:
        return sf::Color(100, 200, 255);
    case Poweruptype::increaseindistance:
        return sf::Color(100, 255, 100);
    case Poweruptype::ballonmode:
        return sf::Color(255, 100, 200);
    case Poweruptype::extralife:
        return sf::Color(255, 60, 60);
    case Poweruptype::Star:
        return sf::Color(255, 255, 0);
    case Poweruptype::Gem:
        return sf::Color(80, 220, 180);
    default:                            return sf::Color::White;
    }

}

char Powerup::getlabelcharacter() const
{
    switch (type)
    {
    case Poweruptype::speedboost:
        return 'S';
    case Poweruptype::snowballpower:
        return 'P';
    case Poweruptype::increaseindistance:
        return 'D';
    case Poweruptype::ballonmode:
        return 'B';
    case Poweruptype::extralife:
        return 'L';
    case Poweruptype::Star:
        return '*';
    case Poweruptype::Gem:
        return 'G';
    default:
        return '?';
    }
}