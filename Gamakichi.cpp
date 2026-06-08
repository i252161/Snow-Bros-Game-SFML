#include "Gamakichi.h"
#include "Constants.h"
#include "TextureManager.h"
#include <cstdlib>

Gamakichi::Gamakichi()
{
    width = 180;
    height = 160;
    hitstoencase = GAMAKICHI_HEALTH;
    remainhits = GAMAKICHI_HEALTH;
    health = GAMAKICHI_HEALTH;
    maxhealth = GAMAKICHI_HEALTH;
    firetimer = 0;
    fireinterval = 2.0f;
    fireReady = false;
    attackphase = 0;
    speed = 0;
    healthBarBg.setSize(sf::Vector2f(240.f, 18.f));
    healthBarBg.setFillColor(sf::Color(60, 60, 60));
    healthBarFill.setSize(sf::Vector2f(240.f, 18.f));
    healthBarFill.setFillColor(sf::Color(220, 80, 20));
}
void Gamakichi::init(float startX, float startY)
{
    x = startX;
    y = startY;
    health = maxhealth;
    remainhits = maxhealth;
    state = Enemystate::Walking;
    alive = true;
    firetimer = 0;
    attackphase = 0;
    sf::Texture& tex = Texturemanager::getinstance().getGamakichi();
    sprite.setTexture(tex);
    int frameW = tex.getSize().x / 5;
    int frameH = tex.getSize().y / 3;
    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    sprite.setScale(width / (float)frameW, height / (float)frameH);

    updatehitbox();
}
void Gamakichi::hitbysnowball()
{
    if (state == Enemystate::Dead)
        return;

    health--;
    remainhits = health;
    updatephase();

    if (health <= 0)
    {
        state = Enemystate::Dead;
        alive = false;
        pickrandomdrop();
    }
}
void Gamakichi::updatephase()
{
    if (health <= maxhealth / 3)
    {
        attackphase = 2;
        fireinterval = 1.0f;
    }
    else if (health <= maxhealth * 2 / 3)
    {
        attackphase = 1;
        fireinterval = 1.5f;
    }
}
void Gamakichi::update(float dt)
{
    if (!alive)
        return;
    firetimer = firetimer + dt;
    if (firetimer >= fireinterval)
    {
        fireReady = true;
        firetimer = 0;
    }
    anime_timer = anime_timer + dt;
    if (anime_timer >= ENEMY_ANIM_SPEED * 2.5f)
    {
        anime_timer = 0;
        animeframe = (animeframe + 1) % 5;
    }

    sf::Texture& tex = Texturemanager::getinstance().getGamakichi();
    int frameW = tex.getSize().x / 5;
    int frameH = tex.getSize().y / 3;
    int row = attackphase;
    sprite.setTextureRect(sf::IntRect(animeframe * frameW, row * frameH, frameW, frameH));
    float healthPct = (float)health / (float)maxhealth;
    healthBarFill.setSize(sf::Vector2f(240.f * healthPct, 18.f));
}
void Gamakichi::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    sprite.setPosition(x, y);
    window.draw(sprite);
    healthBarBg.setPosition(280.f, 8.f);
    window.draw(healthBarBg);
    healthBarFill.setPosition(280.f, 8.f);
    window.draw(healthBarFill);

    drawhitbox(window, sf::Color::Red);
}
bool Gamakichi::wantstofire() const
{
    return fireReady;
}

void Gamakichi::getfireorigins(float origins[][2], int& count) const
{
    if (attackphase == 0)
        count = 2;
    else if (attackphase == 1)
        count = 3;
    else
        count = 4;

    origins[0][0] = x;
    origins[0][1] = y + height / 2.f;
    origins[1][0] = x + width;
    origins[1][1] = y + height / 2.f;
    if (count >= 3)
    {
        origins[2][0] = x + width / 2.f;
        origins[2][1] = y;
    }

    if (count == 4)
    {
        origins[3][0] = x + width / 2.f;
        origins[3][1] = y + height;
    }
}

void Gamakichi::resetfireflag()
{
    fireReady = false;
}
int Gamakichi::gethealth() const
{
    return health;
}
int Gamakichi::getmaxhealth() const
{
    return maxhealth;
}
int Gamakichi::getscore() const
{
    return GAMAKICHI_SCORE;
}
EnemyType Gamakichi::gettype_enemy() const
{
    return EnemyType::Gamakichi;
}