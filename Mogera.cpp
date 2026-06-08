#include "Mogera.h"
#include "Constants.h"
#include "TextureManager.h"
#include <cstdlib>

Mogerachild::Mogerachild()
{
    width = 28;
    height = 28;
    hitstoencase = 1;
    remainhits = 1;
    speed = 2.5f;

    childShape.setRadius(14.f);
    childShape.setFillColor(sf::Color(80, 180, 80));
    childShape.setOutlineColor(sf::Color(40, 120, 40));
    childShape.setOutlineThickness(2.f);
}

void Mogerachild::init(float startX, float startY)
{
    x = startX;
    y = startY;
    movedirection = (rand() % 2 == 0) ? 1 : -1;
    state = Enemystate::Walking;
    remainhits = hitstoencase;
    alive = true;

    sf::Texture& tex = Texturemanager::getinstance().getmogera();
    sprite.setTexture(tex);
    int frameW = tex.getSize().x / 5;
    int frameH = tex.getSize().y / 4;
    sprite.setTextureRect(sf::IntRect(0, 2 * frameH, frameW, frameH));
    sprite.setScale(width / (float)frameW, height / (float)frameH);

    updatehitbox();
}

void Mogerachild::update(float dt)
{
    if (!alive)
        return;

    if (state == Enemystate::Encased || state == Enemystate::Shaking)
    {
        encasedupdate(dt);
        return;
    }
    walkupdate(dt);

    sf::Texture& tex = Texturemanager::getinstance().getmogera();
    int frameW = tex.getSize().x / 5;
    int frameH = tex.getSize().y / 4;
    int col = animeframe % 4;
    sprite.setTextureRect(sf::IntRect(col * frameW, 2 * frameH, frameW, frameH));
}

void Mogerachild::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    sprite.setPosition(x, y);
    window.draw(sprite);
    drawhitbox(window, sf::Color::Red);
}

int Mogerachild::getscore() const
{
    return 200;
}
EnemyType Mogerachild::gettype_enemy() const
{
    return EnemyType::Mogerachild;
}
Mogera::Mogera()
{
    width = 120;
    height = 140;
    hitstoencase = MOGERA_HEALTH;
    remainhits = MOGERA_HEALTH;
    health = MOGERA_HEALTH;
    maxhealth = MOGERA_HEALTH;
    spawntimer = 0;
    spawnChildReady = false;
    speed = 0;
    bodyShape.setSize(sf::Vector2f(120.f, 140.f));
    bodyShape.setFillColor(sf::Color(60, 160, 60));
    healthBarBg.setSize(sf::Vector2f(200.f, 18.f));
    healthBarBg.setFillColor(sf::Color(60, 60, 60));
    healthBarFill.setSize(sf::Vector2f(200.f, 18.f));
    healthBarFill.setFillColor(sf::Color(80, 220, 80));
}
void Mogera::init(float startX, float startY)
{
    x = startX;
    y = startY;
    health = maxhealth;
    remainhits = maxhealth;
    state = Enemystate::Walking;
    alive = true;
    spawntimer = 0;
    sf::Texture& tex = Texturemanager::getinstance().getmogera();
    sprite.setTexture(tex);
    int frameW = tex.getSize().x / 5;
    int frameH = tex.getSize().y / 4;
    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    sprite.setScale(width / (float)frameW, height / (float)frameH);
    updatehitbox();
}
void Mogera::hitbysnowball()
{
    if (state == Enemystate::Dead)
        return;
    health--;
    remainhits = health;
    if (health <= 0)
    {
        state = Enemystate::Dead;
        alive = false;
        pickrandomdrop();
    }
}
void Mogera::update(float dt)
{
    if (!alive)
        return;
    spawntimer = spawntimer + dt;
    if (spawntimer >= BOSS_SPAWN_INTERVAL)
    {
        spawnChildReady = true;
        spawntimer = 0;
    }
    anime_timer = anime_timer + dt;
    if (anime_timer >= ENEMY_ANIM_SPEED * 2)
    {
        anime_timer = 0;
        animeframe = (animeframe + 1) % 3;
    }

    sf::Texture& tex = Texturemanager::getinstance().getmogera();
    int frameW = tex.getSize().x / 5;
    int frameH = tex.getSize().y / 4;
    int row = (health < maxhealth / 2) ? 1 : 0;
    sprite.setTextureRect(sf::IntRect(animeframe * frameW, row * frameH, frameW, frameH));

    float healthPct = (float)health / (float)maxhealth;
    healthBarFill.setSize(sf::Vector2f(200.f * healthPct, 18.f));
    if (healthPct < 0.3f)
        healthBarFill.setFillColor(sf::Color(220, 60, 60));
    else if (healthPct < 0.6f)
        healthBarFill.setFillColor(sf::Color(220, 180, 40));
}
void Mogera::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    sprite.setPosition(x, y);
    window.draw(sprite);
    healthBarBg.setPosition(300.f, 8.f);
    window.draw(healthBarBg);
    healthBarFill.setPosition(300.f, 8.f);
    window.draw(healthBarFill);
    drawhitbox(window, sf::Color::Red);
}
int Mogera::gethealth() const
{
    return health;
}
int Mogera::getmaxhealth() const
{
    return maxhealth;
}
bool Mogera::wantstospawnchild() const
{
    return spawnChildReady;
}
void Mogera::getspawnposition(float& cx, float& cy) const
{
    cx = x + width;
    cy = y + height / 2.f;
}

void Mogera::resetspawnflag()
{
    spawnChildReady = false;
}
int Mogera::getscore() const
{
    return MOGERA_SCORE;
}
EnemyType Mogera::gettype_enemy() const
{
    return EnemyType::Mogera;
}