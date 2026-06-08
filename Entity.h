#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
    Entity();
    virtual ~Entity();

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    float getX() const;
    float getY() const;
    float getwidth() const;
    float getheight() const;

    void setposition(float x, float y);
    bool isalive() const;
    void setalive(bool val);

    sf::FloatRect gethitbox() const;
    static void toggleHitboxDebug();
    bool ishitboxdebug() const;

protected:
    float x;
    float y;
    float width;
    float height;
    float velX;
    float velY;
    bool alive;
    bool onground;
    bool facingright;

    sf::Sprite sprite;
    sf::FloatRect hitbox;

    static bool showhitboxes;

    void applygravity(float dt);
    void drawhitbox(sf::RenderWindow& window, sf::Color color);
    void updatehitbox();
};