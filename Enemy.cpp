#include "Enemy.h"
#include "Constants.h"
#include <cstdlib>

Enemy::Enemy()
{
    state = Enemystate::Walking;
    hitstoencase = BOTOM_HITS_TO_ENCASE;
    remainhits = hitstoencase;
    statetimer = 0;
    anime_timer = 0;
    animeframe = 0;
    movedirection = 1;
    speed = 1.8f;
    shoulddroppickup = false;
    droptype = Poweruptype::Gem;
}
Enemy::~Enemy()
{
}
void Enemy::hitbysnowball()
{
    if (state == Enemystate::Dead || state == Enemystate::Encased)
        return;
    remainhits--;
    if (remainhits <= 0)
    {
        state = Enemystate::Encased;
        statetimer = 0;
        velX = 0;
        velY = 0;
        pickrandomdrop();
    }
}
bool Enemy::isencased() const
{
    return state == Enemystate::Encased || state == Enemystate::Shaking;
}
bool Enemy::isshaking() const
{
    return state == Enemystate::Shaking;
}
void Enemy::startrolling()
{
    state = Enemystate::Dead;
    alive = false;
}
int Enemy::gethitsremaining() const
{
    return remainhits;
}
Enemystate Enemy::getstate() const
{
    return state;
}
void Enemy::collidingplatforms(Platform* platforms, int count)
{
    landOnplatform(platforms, count);
    collidesides(platforms, count);
}
bool Enemy::dropspickup() const
{
    return shoulddroppickup;
}
Poweruptype Enemy::getdroptype() const
{
    return droptype;
}
void Enemy::walkupdate(float dt)
{
    applygravity(dt);
    x = x + movedirection * speed;
    y = y + velY;
    if (x < 0)
    {
        x = 0;
        movedirection = 1;
    }
    else if (x + width > 800)
    {
        x = 800 - width;
        movedirection = -1;
    }
    anime_timer = anime_timer + dt;
    if (anime_timer >= ENEMY_ANIM_SPEED)
    {
        anime_timer = 0;
        animeframe = (animeframe + 1) % 4;
    }

    updatehitbox();
}
void Enemy::encasedupdate(float dt)
{
    statetimer = statetimer + dt;

    applygravity(dt);
    y = y + velY;

    if (statetimer >= ENCASE_SHAKE_TIME * 0.7f && state == Enemystate::Encased)
    {
        state = Enemystate::Shaking;
    }

    if (statetimer >= ENCASE_SHAKE_TIME)
    {
        state = Enemystate::Walking;
        remainhits = hitstoencase;
        statetimer = 0;
    }

    updatehitbox();
}
void Enemy::pickrandomdrop()
{
    int roll = rand() % 10;
    shoulddroppickup = true;

    if (roll < 4)
        droptype = Poweruptype::Gem;
    else if (roll < 6)
        droptype = Poweruptype::speedboost;
    else if (roll < 7)
        droptype = Poweruptype::snowballpower;
    else if (roll < 8)
        droptype = Poweruptype::increaseindistance;
    else if (roll < 9)
        droptype = Poweruptype::ballonmode;
    else
        droptype = Poweruptype::extralife;
}
void Enemy::landOnplatform(Platform* platforms, int count)
{
    onground = false;
    for (int i = 0; i < count; i++)
    {
        sf::FloatRect pRect = platforms[i].getrect();

        if (hitbox.intersects(pRect))
        {
            float previousbottom = y + height - velY;
            if (velY >= 0 && previousbottom <= pRect.top + 6)
            {
                y = pRect.top - height;
                velY = 0;
                onground = true;
                break;
            }
        }
    }
}

void Enemy::collidesides(Platform* platforms, int count)
{
    for (int i = 0; i < count; i++)
    {
        sf::FloatRect pRect = platforms[i].getrect();

        if (hitbox.intersects(pRect))
        {
            if (x + width > pRect.left && x < pRect.left && velX > 0)
            {
                x = pRect.left - width;
                movedirection = -1;
            }
            else if (x < pRect.left + pRect.width && x + width > pRect.left + pRect.width && velX < 0)
            {
                x = pRect.left + pRect.width;
                movedirection = 1;
            }
        }
    }
}