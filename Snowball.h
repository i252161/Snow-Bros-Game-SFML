#pragma once
#include "Entity.h"
#include "Platform.h"

class Snowball : public Entity
{
public:
    Snowball();

    void launch(float startX, float startY, bool goingRight, float speedMult);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void collidingplatforms(Platform* platforms, int count);

    bool isRolling() const;
    void startrolling();
    int getkillchain() const;
    void addkill();

    bool hasHitEnemy() const;
    void setHitEnemy(bool val);

    int getownerindex() const;
    void setownerindex(int idx);

private:
    bool rolling;
    bool enemyhit;
    int killingchain;
    int ownerindex;
    float lifetime;

    sf::CircleShape snowShape;
    sf::CircleShape rollingShape;

    void wrapscreen();
};