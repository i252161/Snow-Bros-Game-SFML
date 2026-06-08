#include "Entity.h"
#include "Constants.h"

bool Entity::showhitboxes = false;

Entity::Entity()
{
    x = 0;
    y = 0;
    width = 40;
    height = 48;
    velX = 0;
    velY = 0;
    alive = true;
    onground = false;
    facingright = true;
}
Entity::~Entity()
{
}
float Entity::getX() const
{
    return x;
}
float Entity::getY() const
{
    return y;
}
float Entity::getwidth() const
{
    return width;
}
float Entity::getheight() const
{
    return height;
}
void Entity::setposition(float px, float py)
{
    x = px;
    y = py;
    updatehitbox();
}
bool Entity::isalive() const
{
    return alive;
}
void Entity::setalive(bool val)
{
    alive = val;
}

sf::FloatRect Entity::gethitbox() const
{
    return hitbox;
}

void Entity::toggleHitboxDebug()
{
    showhitboxes = !showhitboxes;
}

bool Entity::ishitboxdebug() const
{
    return showhitboxes;
}
void Entity::applygravity(float dt)
{
    if (!onground)
    {
        velY += GRAVITY;
        if (velY > 18.0f)
            velY = 18.0f;
    }
}
void Entity::updatehitbox()
{
    hitbox.left = x;
    hitbox.top = y;
    hitbox.width = width;
    hitbox.height = height;
}
void Entity::drawhitbox(sf::RenderWindow& window, sf::Color color)
{
    if (!showhitboxes)
        return;
    sf::RectangleShape box;
    box.setPosition(hitbox.left, hitbox.top);
    box.setSize(sf::Vector2f(hitbox.width, hitbox.height));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(color);
    box.setOutlineThickness(1.5f);
    window.draw(box);
}