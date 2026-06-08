#pragma once
#include "Enemy.h"

class Botom : public Enemy
{
public:
    Botom();

    void init(float startX, float startY) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    int getscore() const override;
    EnemyType gettype_enemy() const override;

    void setvariant(int v);

private:
    int variant;
    float jumptimer;
    float jumpinterval;

    sf::RectangleShape bodyShape;
    sf::CircleShape eyeShape;
    sf::RectangleShape encasedShape;

    void drawwalking(sf::RenderWindow& window);
    void drawencased(sf::RenderWindow& window);
    void tryJjump();
};