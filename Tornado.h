#pragma once
#include "Enemy.h"
#include "Projectile.h"

class Tornado : public Enemy
{
public:
    Tornado();

    void init(float startX, float startY) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    int getscore() const override;
    EnemyType gettype_enemy() const override;

    void setvariant(int v);
    void setpositionplayer(float px, float py);

    bool firedknife() const;
    void getorigin_knife(float& kx, float& ky) const;
    void resetknifeflag();

private:
    int variant;
    float knifetimer;
    float flighttimer;
    float flightduration;
    float groundtimer;
    float flighttargetX;
    float flighttargetY;
    float playerX;
    float playerY;
    bool fireknifestart;
    float currentspeed_flight;

    sf::RectangleShape encasedShape;

    void updateground(float dt);
    void updateflight(float dt);
    void pickflighttarget();
    void drawencased(sf::RenderWindow& window);
};