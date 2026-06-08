#include "Tornado.h"
#include "Constants.h"
#include "TextureManager.h"
#include <cstdlib>
#include <cmath>

Tornado::Tornado()
{
    width = 40;
    height = 44;
    hitstoencase = tornadohits_encase;
    remainhits = hitstoencase;
    speed = 2.2f;
    variant = 0;
    knifetimer = 0;
    flighttimer = 0;
    flightduration = 0;
    groundtimer = 0;
    flighttargetX = 400;
    flighttargetY = 200;
    playerX = 400;
    playerY = 400;
    fireknifestart = false;
    currentspeed_flight = 2.5f;
    encasedShape.setSize(sf::Vector2f(44.f, 48.f));
    encasedShape.setFillColor(sf::Color(210, 235, 255, 200));
    encasedShape.setOutlineColor(sf::Color(140, 200, 255));
    encasedShape.setOutlineThickness(2.f);
}
void Tornado::setvariant(int v)
{
    variant = v;
    if (v == 1)
    {
        speed = 2.8f;
        hitstoencase = 4;
        remainhits = 4;
    }
    else if (v == 2)
    {
        speed = 3.4f;
        hitstoencase = 5;
        remainhits = 5;
    }
}
void Tornado::init(float startX, float startY)
{
    x = startX;
    y = startY;
    movedirection = (rand() % 2 == 0) ? 1 : -1;
    state = Enemystate::Walking;
    remainhits = hitstoencase;
    alive = true;
    groundtimer = flight_interval * 0.3f + (rand() % 15) / 10.0f;
    knifetimer = tornadoknife_interval * 0.5f;
    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getTornadoRed();
    else
        tex = &Texturemanager::getinstance().getTornadoBlue();
    sprite.setTexture(*tex);
    int frameW = tex->getSize().x / 2;
    int frameH = tex->getSize().y / 5;
    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    float scaleX = width / (float)frameW;
    float scaleY = height / (float)frameH;
    sprite.setScale(scaleX, scaleY);
    updatehitbox();
}

void Tornado::setpositionplayer(float px, float py)
{
    playerX = px;
    playerY = py;
}

void Tornado::update(float dt)
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
    knifetimer += dt;
    if (knifetimer >= tornadoknife_interval)
    {
        fireknifestart = true;
        knifetimer = 0;
    }
    if (state == Enemystate::Flying)
        updateflight(dt);
    else
        updateground(dt);
}

void Tornado::updateground(float dt)
{
    walkupdate(dt);

    groundtimer += dt;
    if (groundtimer >= flight_interval * 0.6f)
    {
        state = Enemystate::Flying;
        flighttimer = 0;
        currentspeed_flight = 1.5f + (rand() % 40) / 10.0f;
        flightduration = 2.0f + (rand() % 20) / 10.0f;
        groundtimer = 0;
        pickflighttarget();
    }

    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getTornadoRed();
    else
        tex = &Texturemanager::getinstance().getTornadoBlue();

    int frameW = tex->getSize().x / 2;
    int frameH = tex->getSize().y / 5;
    sprite.setTextureRect(sf::IntRect((animeframe % 2) * frameW, 0, frameW, frameH));
}

void Tornado::updateflight(float dt)
{
    flighttimer = flighttimer + dt;
    float dx = flighttargetX - x;
    float dy = flighttargetY - y;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < 8.0f)
    {
        pickflighttarget();
    }
    else
    {
        velX = (dx / dist) * currentspeed_flight;
        velY = (dy / dist) * currentspeed_flight;
        x += velX;
        y += velY;
    }
    if (x < 0) x = 0;
    if (x + width > 800) x = 800 - width;
    if (y < 30) y = 30;
    if (y + height > 580) y = 580 - height;
    if (flighttimer >= flightduration)
    {
        state = Enemystate::Walking;
        groundtimer = 0;
        velX = 0;
        velY = 0;
    }
    anime_timer = anime_timer + dt;
    if (anime_timer >= ENEMY_ANIM_SPEED * 0.7f)
    {
        anime_timer = 0;
        animeframe = (animeframe + 1) % 4;
    }
    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getTornadoRed();
    else
        tex = &Texturemanager::getinstance().getTornadoBlue();

    int frameW = tex->getSize().x / 2;
    int frameH = tex->getSize().y / 5;
    int row = 1 + animeframe % 3;
    sprite.setTextureRect(sf::IntRect(0, row * frameH, frameW, frameH));
    updatehitbox();
}
void Tornado::pickflighttarget()
{
    flighttargetX = 50.0f + rand() % 700;
    flighttargetY = 60.0f + rand() % 450;
}
void Tornado::draw(sf::RenderWindow& window)
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
void Tornado::drawencased(sf::RenderWindow& window)
{
    encasedShape.setPosition(x - 2, y - 2);

    if (state == Enemystate::Shaking)
    {
        float shake = (rand() % 6) - 3;
        encasedShape.move(shake, 0);
        encasedShape.setFillColor(sf::Color(255, 200, 180, 180));
    }
    else
    {
        encasedShape.setFillColor(sf::Color(210, 235, 255, 200));
    }
    window.draw(encasedShape);
    drawhitbox(window, sf::Color::Red);
}
bool Tornado::firedknife() const
{
    return fireknifestart;
}

void Tornado::getorigin_knife(float& kx, float& ky) const
{
    kx = x + width / 2.f;
    ky = y + height / 2.f;
}

void Tornado::resetknifeflag()
{
    fireknifestart = false;
}

int Tornado::getscore() const
{
    return TORNADO_SCORE_MIN + rand() % (TORNADO_SCORE_MAX - TORNADO_SCORE_MIN + 1);
}

EnemyType Tornado::gettype_enemy() const
{
    return EnemyType::Tornado;
}