#pragma once
#include "Entity.h"
#include "Snowball.h"
#include "Platform.h"
#include "PowerUp.h"

enum class PlayerCharacter
{
    Nick,
    Blue,
    Green,
    Red,
    Yellow
};

struct PlayerState
{
    int lives;
    int gems;
    int score;
    int currentLevel;
    bool speedBoostActive;
    bool snowballPowerActive;
    bool distanceBoostActive;
    bool balloonActive;
    float speedBoostTimer;
    float balloonTimer;
};

class Player : public Entity
{
public:
    Player();
    bool isFacingRight() const;


    void init(float startX, float startY, int playerIndex, PlayerCharacter ch);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void handleInput(float dt);
    void collidingplatforms(Platform* platforms, int count);

    bool tryFireSnowball();
    void resetfireflag();
    bool wantstofire() const;

    void loselife();
    void addlife();
    void addgems(int amount);
    void addscore(int amount);

    void applypowerup(Poweruptype type);
    void clearlevelpowerups();

    PlayerState& getstate();
    int getplayerindex() const;
    PlayerCharacter getcharacter() const;

    float getSnowballSpeedMult() const;
    float getSnowballDistMult() const;

    void respawn(float sx, float sy);
    bool isinvincible() const;

    void setControls(sf::Keyboard::Key left, sf::Keyboard::Key right,
        sf::Keyboard::Key jump, sf::Keyboard::Key fire);

private:
    PlayerState state;
    int playerIndex;
    PlayerCharacter character;

    sf::Keyboard::Key keyLeft;
    sf::Keyboard::Key keyRight;
    sf::Keyboard::Key keyJump;
    sf::Keyboard::Key keyFire;

    bool wantFire;
    float fireCooldown;
    float invincibletimer;
    float anime_timer;
    int animeframe;
    int animRow;

    float snowballSpeedMult;
    float snowballDistMult;

    void updatepoweruptimers(float dt);
    void updateanimation(float dt, bool moving, bool jumping);
    void drawPlayer(sf::RenderWindow& window);
    void setTextureForCharacter();
};