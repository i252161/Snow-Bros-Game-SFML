#include "Snowball.h"
#include "Constants.h"

Snowball::Snowball()
{
    width = 18;
    height = 18;
    rolling = false;
    enemyhit = false;
    killingchain = 0;
    ownerindex = 0;
    lifetime = 0;
    alive = false;
    snowShape.setRadius(9.f);
    snowShape.setFillColor(sf::Color(220, 240, 255));
    snowShape.setOutlineColor(sf::Color(150, 200, 255));
    snowShape.setOutlineThickness(1.5f);
    rollingShape.setRadius(12.f);
    rollingShape.setFillColor(sf::Color(200, 230, 255));
    rollingShape.setOutlineColor(sf::Color(100, 170, 240));
    rollingShape.setOutlineThickness(2.f);
}
void Snowball::launch(float startX, float startY, bool goingRight, float speedMult)
{
    x = startX;
    y = startY;
    velY = 0;
    if (goingRight)
        velX = SNOWBALL_SPEED * speedMult;
    else
        velX = -SNOWBALL_SPEED * speedMult;
    rolling = false;
    enemyhit = false;
    killingchain = 0;
    lifetime = 0;
    alive = true;
    onground = false;
    updatehitbox();
}
void Snowball::update(float dt)
{
    if (!alive)
        return;

    lifetime = lifetime + dt;

    if (!rolling)
    {
        if (lifetime > 3.5f)
        {
            alive = false;
            return;
        }
        x = x + velX;
        wrapscreen();
    }
    else
    {
        if (lifetime > 8.0f)
        {
            alive = false;
            return;
        }
        applygravity(dt);
        x = x + velX;
        y = y + velY;
        wrapscreen();
        float radius = 12.f + killingchain * 2.f;
        if (radius > 30.f)
            radius = 30.f;
        rollingShape.setRadius(radius);
    }
    updatehitbox();
}

void Snowball::collidingplatforms(Platform* platforms, int count)
{
    if (!rolling || !alive)
        return;

    for (int i = 0; i < count; i++)
    {
        sf::FloatRect pRect = platforms[i].getrect();
        if (hitbox.intersects(pRect))
        {
            if (velY > 0 && y + height - velY <= pRect.top + 4)
            {
                y = pRect.top - height;
                velY = 0;
                onground = true;
            }
        }
    }
    updatehitbox();
}
void Snowball::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    if (!rolling)
    {
        snowShape.setPosition(x, y);
        window.draw(snowShape);
    }
    else
    {
        rollingShape.setPosition(x - rollingShape.getRadius() + 9, y - rollingShape.getRadius() + 9);
        window.draw(rollingShape);
    }
    drawhitbox(window, sf::Color::Yellow);
}

void Snowball::wrapscreen()
{
    if (x + width < 0)
        x = 800;
    else if (x > 800)
        x = -width;
}

bool Snowball::isRolling() const
{
    return rolling;
}

void Snowball::startrolling()
{
    rolling = true;
    lifetime = 0;
    velY = -3.0f;
    width = 24;
    height = 24;
    updatehitbox();
}

int Snowball::getkillchain() const
{
    return killingchain;
}
void Snowball::addkill()
{
    killingchain++;
}
bool Snowball::hasHitEnemy() const
{
    return enemyhit;
}
void Snowball::setHitEnemy(bool val)
{
    enemyhit = val;
}
int Snowball::getownerindex() const
{
    return ownerindex;
}
void Snowball::setownerindex(int idx)
{
    ownerindex = idx;
}