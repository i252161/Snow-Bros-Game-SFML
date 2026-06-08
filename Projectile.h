#pragma once
#include "Entity.h"

enum class ProjectileType
{
    Knife,
    Artillery
};

class Projectile : public Entity
{
public:
    Projectile();

    void launch(float startX, float startY, float targetX, float targetY, ProjectileType t);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    ProjectileType gettype() const;

private:
    ProjectileType type;
    float lifetime;
    float angle;

    sf::RectangleShape knifeShape;
    sf::CircleShape artilleryShape;
};