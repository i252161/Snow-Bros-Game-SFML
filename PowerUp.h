#pragma once
#include "Entity.h"

enum class Poweruptype
{
    speedboost,
    snowballpower,
    increaseindistance,
    ballonmode,
    extralife,
    Gem,
    Star
};

class Powerup : public Entity
{
public:
    Powerup();

    void spawn(float px, float py, Poweruptype type);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    Poweruptype gettype() const;
    bool iscollected() const;
    void collect();

private:
    Poweruptype type;
    bool collected;
    float lifetime;
    float boboffset;
    float bobtimer;

    sf::CircleShape shape;
    sf::Text label;

    sf::Color getcolorForType() const;
    char getlabelcharacter() const;
};