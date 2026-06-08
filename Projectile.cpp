#include "Projectile.h"
#include "Constants.h"
#include <cmath>

Projectile::Projectile()
{
    width = 12;
    height = 6;
    alive = false;
    lifetime = 0;
    angle = 0;
    type = ProjectileType::Knife;

    knifeShape.setSize(sf::Vector2f(18.f, 5.f));
    knifeShape.setFillColor(sf::Color(200, 200, 200));
    knifeShape.setOutlineColor(sf::Color(100, 100, 100));
    knifeShape.setOutlineThickness(1.f);
    knifeShape.setOrigin(9.f, 2.5f);

    artilleryShape.setRadius(8.f);
    artilleryShape.setFillColor(sf::Color(220, 80, 20));
    artilleryShape.setOutlineColor(sf::Color(150, 40, 10));
    artilleryShape.setOutlineThickness(1.5f);
}

void Projectile::launch(float startX, float startY, float targetX, float targetY, ProjectileType t)
{
    x = startX;
    y = startY;
    type = t;
    alive = true;
    lifetime = 0;

    float dx = targetX - startX;
    float dy = targetY - startY;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 1.0f) dist = 1.0f;

    float speed = (t == ProjectileType::Knife) ? KNIFE_SPEED : 4.5f;
    velX = (dx / dist) * speed;
    velY = (dy / dist) * speed;

    angle = std::atan2(dy, dx) * 180.f / 3.14159f;

    updatehitbox();
}

void Projectile::update(float dt)
{
    if (!alive)
        return;

    lifetime += dt;
    if (lifetime > 4.0f)
    {
        alive = false;
        return;
    }

    x += velX;
    y += velY;

    if (x < -20 || x > 820 || y < -20 || y > 620)
        alive = false;

    updatehitbox();
}

void Projectile::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;

    if (type == ProjectileType::Knife)
    {
        knifeShape.setPosition(x + width / 2.f, y + height / 2.f);
        knifeShape.setRotation(angle);
        window.draw(knifeShape);
    }
    else
    {
        artilleryShape.setPosition(x, y);
        window.draw(artilleryShape);
    }

    drawhitbox(window, sf::Color::Yellow);
}

ProjectileType Projectile::gettype() const { return type; }