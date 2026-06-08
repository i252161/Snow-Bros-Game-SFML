#include "Botom.h"
#include "Constants.h"
#include "TextureManager.h"
#include <cstdlib>
#include <cmath>

Botom::Botom()
{
    width = 36;
    height = 42;
    hitstoencase = BOTOM_HITS_TO_ENCASE;
    remainhits = hitstoencase;
    speed = 1.6f;
    variant = 0;
    jumptimer = 0;
    jumpinterval = 2.5f + (rand() % 30) / 10.0f;

    bodyShape.setSize(sf::Vector2f(36.f, 42.f));
    eyeShape.setRadius(5.f);
    encasedShape.setSize(sf::Vector2f(40.f, 46.f));
    encasedShape.setFillColor(sf::Color(180, 220, 255, 200));
    encasedShape.setOutlineColor(sf::Color(100, 180, 255));
    encasedShape.setOutlineThickness(2.f);
}
void Botom::setvariant(int v)
{
    variant = v;
    if (v == 1)
    {
        speed = 2.0f;
        hitstoencase = 3;
        remainhits = 3;
    }
    else if (v == 2)
    {
        speed = 2.4f;
        hitstoencase = 4;
        remainhits = 4;
    }
    else if (v == 3)
    {
        speed = 2.8f;
        hitstoencase = 5;
        remainhits = 5;
    }
}
void Botom::init(float startX, float startY)
{
    x = startX;
    y = startY;
    movedirection = (rand() % 2 == 0) ? 1 : -1;
    state = Enemystate::Walking;
    remainhits = hitstoencase;
    alive = true;
    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getBotomBlue();
    else if (variant == 1)
        tex = &Texturemanager::getinstance().getBotomOrange();
    else
        tex = &Texturemanager::getinstance().getBotomPink();
    sprite.setTexture(*tex);
    int frameW = tex->getSize().x / 2;
    int frameH = tex->getSize().y / 6;
    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    float scaleX = width / (float)frameW;
    float scaleY = height / (float)frameH;
    sprite.setScale(scaleX, scaleY);
    updatehitbox();
}
void Botom::update(float dt)
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
    walkupdate(dt);
    jumptimer += dt;
    if (jumptimer >= jumpinterval && onground)
    {
        tryJjump();
        jumptimer = 0;
        jumpinterval = 2.0f + (rand() % 30) / 10.0f;
    }
    sf::Texture* tex = nullptr;
    if (variant == 0)
        tex = &Texturemanager::getinstance().getBotomBlue();
    else if (variant == 1)
        tex = &Texturemanager::getinstance().getBotomOrange();
    else
        tex = &Texturemanager::getinstance().getBotomPink();
    int frameW = tex->getSize().x / 2;
    int frameH = tex->getSize().y / 6;
    int col = animeframe % 2;
    int row = onground ? 0 : 1;
    sprite.setTextureRect(sf::IntRect(col * frameW, row * frameH, frameW, frameH));
    if (movedirection < 0)
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    else
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
}
void Botom::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    if (state == Enemystate::Encased || state == Enemystate::Shaking)
    {
        drawencased(window);
        return;
    }
    drawwalking(window);
    drawhitbox(window, sf::Color::Red);
}

void Botom::drawwalking(sf::RenderWindow& window)
{
    if (movedirection < 0)
        sprite.setPosition(x + width, y);
    else
        sprite.setPosition(x, y);

    window.draw(sprite);
}

void Botom::drawencased(sf::RenderWindow& window)
{
    encasedShape.setPosition(x - 2, y - 2);
    if (state == Enemystate::Shaking)
    {
        float shake = (rand() % 6) - 3;
        encasedShape.move(shake, 0);
        encasedShape.setFillColor(sf::Color(255, 220, 200, 180));
    }
    else
    {
        encasedShape.setFillColor(sf::Color(180, 220, 255, 200));
    }
    window.draw(encasedShape);
    drawhitbox(window, sf::Color::Red);
}
void Botom::tryJjump()
{
    if (onground)
    {
        velY = -8.0f;
        onground = false;
    }
}

int Botom::getscore() const
{
    return BOTOM_SCORE_MIN + rand() % (BOTOM_SCORE_MAX - BOTOM_SCORE_MIN + 1);
}

EnemyType Botom::gettype_enemy() const
{
    return EnemyType::Botom;
}