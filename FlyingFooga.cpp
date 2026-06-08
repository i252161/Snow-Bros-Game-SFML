#include "FlyingFooga.h"
#include "Constants.h"
#include "TextureManager.h"
#include <cstdlib>
#include <cmath>

FlyingFooga::FlyingFooga()
{
    width = 38;
    height = 40;
    hitstoencase = FOOGA_HITS_TO_ENCASE;
    remainhits = hitstoencase;
    speed = 2.0f;
    variant = 0;
    flighttimer = 0;
    flightduration = 0;
    groundtimer = 0;
    flighttargetX = 400;
    flighttargetY = 200;
    encasedShape.setSize(sf::Vector2f(42.f, 44.f));
    encasedShape.setFillColor(sf::Color(200, 230, 255, 200));
    encasedShape.setOutlineColor(sf::Color(120, 190, 255));
    encasedShape.setOutlineThickness(2.f);
}
void FlyingFooga::setvariant(int v)
{
    variant = v;
    if (v == 1)
    {
        speed = 2.5f;
        hitstoencase = 3;
        remainhits = 3;
    }
    else if (v == 2)
    {
        speed = 3.0f;
        hitstoencase = 4;
        remainhits = 4;
    }
}
void FlyingFooga::init(float startX, float startY)
{
    x = startX;
    y = startY;
    movedirection = (rand() % 2 == 0) ? 1 : -1;
    state = Enemystate::Walking;
    remainhits = hitstoencase;
    alive = true;
    groundtimer = flight_interval * 0.5f + (rand() % 20) / 10.0f;

    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getFoogaRed();
    else if (variant == 1)
        tex = &Texturemanager::getinstance().getFoogaOrange();
    else
        tex = &Texturemanager::getinstance().getFoogaBlue();

    sprite.setTexture(*tex);
    int frameW = tex->getSize().x;
    int frameH = tex->getSize().y / 8;
    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));

    float scaleX = width / (float)frameW;
    float scaleY = height / (float)frameH;
    sprite.setScale(scaleX, scaleY);

    updatehitbox();
}

void FlyingFooga::update(float dt)
{
    if (!alive)
        return;

    if (state == Enemystate::Encased || state == Enemystate::Shaking)
    {
        encasedupdate(dt);
        return;
    }

    if (state == Enemystate::Dead)
        return;

    if (state == Enemystate::Flying)
    {
        updateflight(dt);
    }
    else
    {
        updateground(dt);
    }
}

void FlyingFooga::updateground(float dt)
{
    walkupdate(dt);

    groundtimer += dt;
    if (groundtimer >= flight_interval)
    {
        state = Enemystate::Flying;
        flighttimer = 0;
        flightduration = FLIGHT_DURATION + (rand() % 20) / 10.0f;
        groundtimer = 0;
        pickflighttarget();
    }

    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getFoogaRed();
    else if (variant == 1)
        tex = &Texturemanager::getinstance().getFoogaOrange();
    else
        tex = &Texturemanager::getinstance().getFoogaBlue();

    int frameW = tex->getSize().x;
    int frameH = tex->getSize().y / 8;
    sprite.setTextureRect(sf::IntRect(0, animeframe % 3 * frameH, frameW, frameH));
}

void FlyingFooga::updateflight(float dt)
{
    flighttimer += dt;

    float dx = flighttargetX - x;
    float dy = flighttargetY - y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 5.0f)
    {
        pickflighttarget();
    }
    else
    {
        float flySpeed = speed * 1.8f;
        velX = (dx / dist) * flySpeed;
        velY = (dy / dist) * flySpeed;
        x += velX;
        y += velY;
    }

    if (x < 0) x = 0;
    if (x + width > 800) x = 800 - width;
    if (y < 30) y = 30;
    if (y + height > 580) y = 580 - height;

    anime_timer = anime_timer + dt;
    if (anime_timer >= ENEMY_ANIM_SPEED)
    {
        anime_timer = 0;
        animeframe = (animeframe + 1) % 4;
    }

    if (flighttimer >= flightduration)
    {
        state = Enemystate::Walking;
        groundtimer = 0;
        velX = 0;
        velY = 0;
    }

    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getFoogaRed();
    else if (variant == 1)
        tex = &Texturemanager::getinstance().getFoogaOrange();
    else
        tex = &Texturemanager::getinstance().getFoogaBlue();
    int frameW = tex->getSize().x;
    int frameH = tex->getSize().y / 8;
    int row = 3 + animeframe % 3;
    sprite.setTextureRect(sf::IntRect(0, row * frameH, frameW, frameH));
    updatehitbox();
}

void FlyingFooga::pickflighttarget()
{
    flighttargetX = 50.0f + rand() % 700;
    flighttargetY = 60.0f + rand() % 450;
}

void FlyingFooga::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    if (state == Enemystate::Encased || state == Enemystate::Shaking)
    {
        drawencased(window);
        return;
    }

    if (movedirection < 0 && state != Enemystate::Flying)
        sprite.setPosition(x + width, y);
    else
        sprite.setPosition(x, y);
    window.draw(sprite);
    drawhitbox(window, sf::Color::Red);
}

void FlyingFooga::drawencased(sf::RenderWindow& window)
{
    encasedShape.setPosition(x - 2, y - 2);

    if (state == Enemystate::Shaking)
    {
        float shake = (rand() % 6) - 3;
        encasedShape.move(shake, 0);
        encasedShape.setFillColor(sf::Color(255, 210, 190, 180));
    }
    window.draw(encasedShape);
    drawhitbox(window, sf::Color::Red);
}
int FlyingFooga::getscore() const
{
    return FOOGA_SCORE_MIN + rand() % (FOOGA_SCORE_MAX - FOOGA_SCORE_MIN + 1);
}

EnemyType FlyingFooga::gettype_enemy() const
{
    return EnemyType::FlyingFooga;
}