#pragma once
#include "Enemy.h"
#include "Projectile.h"

class Gamakichi : public Enemy
{
public:
    Gamakichi();

    void init(float startX, float startY) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    int getscore() const override;
    EnemyType gettype_enemy() const override;

    int gethealth() const;
    int getmaxhealth() const;

    bool wantstofire() const;
    void getfireorigins(float origins[][2], int& count) const;
    void resetfireflag();
    void hitbysnowball();

private:
    int health;
    int maxhealth;
    float firetimer;
    float fireinterval;
    bool fireReady;
    int attackphase;

    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBarFill;

    void updatephase();
};