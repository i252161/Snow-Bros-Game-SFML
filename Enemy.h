#pragma once
#include "Entity.h"
#include "Platform.h"
#include "PowerUp.h"
#include "Projectile.h"

enum class Enemystate
{
    Walking,
    Flying,
    Encased,
    Shaking,
    Dead
};

enum class EnemyType
{
    Botom,
    FlyingFooga,
    Tornado,
    Mogera,
    Mogerachild,
    Gamakichi
};

class Enemy : public Entity
{
public:
    Enemy();
    virtual ~Enemy();

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void init(float startX, float startY) = 0;

    virtual int getscore() const = 0;
    virtual EnemyType gettype_enemy() const = 0;

    void hitbysnowball();
    bool isencased() const;
    bool isshaking() const;
    void startrolling();

    int gethitsremaining() const;
    Enemystate getstate() const;

    void collidingplatforms(Platform* platforms, int count);

    bool dropspickup() const;
    Poweruptype getdroptype() const;

protected:
    Enemystate state;
    int hitstoencase;
    int remainhits;
    float statetimer;
    float anime_timer;
    int animeframe;
    int movedirection;
    float speed;

    bool shoulddroppickup;
    Poweruptype droptype;

    void walkupdate(float dt);
    void encasedupdate(float dt);
    void pickrandomdrop();
    void collidesides(Platform* platforms, int count);
    void landOnplatform(Platform* platforms, int count);
};