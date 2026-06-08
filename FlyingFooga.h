#pragma once
#include "Enemy.h"

class FlyingFooga : public Enemy
{
public:
    FlyingFooga();

    void init(float startX, float startY) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    int getscore() const override;
    EnemyType gettype_enemy() const override;

    void setvariant(int v);

private:
    int variant;
    float flighttimer;
    float flightduration;
    float groundtimer;

    float flighttargetX;
    float flighttargetY;

    sf::RectangleShape encasedShape;

    void updateground(float dt);
    void updateflight(float dt);
    void pickflighttarget();
    void drawencased(sf::RenderWindow& window);
};