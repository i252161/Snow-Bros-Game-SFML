#pragma once
#include "Enemy.h"

class Mogerachild : public Enemy
{
public:
    Mogerachild();
    void init(float startX, float startY) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    int getscore() const override;
    EnemyType gettype_enemy() const override;

private:
    sf::CircleShape childShape;
};

class Mogera : public Enemy
{
public:
    Mogera();

    void init(float startX, float startY) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    int getscore() const override;
    EnemyType gettype_enemy() const override;

    int gethealth() const;
    int getmaxhealth() const;

    bool wantstospawnchild() const;
    void getspawnposition(float& cx, float& cy) const;
    void resetspawnflag();
    void hitbysnowball();

private:
    int health;
    int maxhealth;
    float spawntimer;
    bool spawnChildReady;

    sf::RectangleShape bodyShape;
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBarFill;
};