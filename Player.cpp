#include "Player.h"
#include "Constants.h"
#include "TextureManager.h"
#include <cmath>

Player::Player()
{
    width = 40;
    height = 52;
    wantFire = false;
    fireCooldown = 0;
    invincibletimer = 0;
    anime_timer = 0;
    animeframe = 0;
    animRow = 0;
    playerIndex = 0;
    character = PlayerCharacter::Nick;
    snowballSpeedMult = 1.0f;
    snowballDistMult = 1.0f;

    state.lives = START_LIVES;
    state.gems = 0;
    state.score = 0;
    state.currentLevel = 1;
    state.speedBoostActive = false;
    state.snowballPowerActive = false;
    state.distanceBoostActive = false;
    state.balloonActive = false;
    state.speedBoostTimer = 0;
    state.balloonTimer = 0;

    keyLeft = sf::Keyboard::A;
    keyRight = sf::Keyboard::D;
    keyJump = sf::Keyboard::W;
    keyFire = sf::Keyboard::Space;
}

void Player::init(float startX, float startY, int idx, PlayerCharacter ch)
{
    x = startX;
    y = startY;
    playerIndex = idx;
    character = ch;
    alive = true;
    velX = 0;
    velY = 0;
    onground = false;
    wantFire = false;
    fireCooldown = 0;
    invincibletimer = 0;
    animeframe = 0;
    animRow = 0;

    setTextureForCharacter();
    updatehitbox();
}

void Player::setControls(sf::Keyboard::Key left, sf::Keyboard::Key right,
    sf::Keyboard::Key jump, sf::Keyboard::Key fire)
{
    keyLeft = left;
    keyRight = right;
    keyJump = jump;
    keyFire = fire;
}

void Player::setTextureForCharacter()
{
    sf::Texture* tex = nullptr;

    switch (character)
    {
    case PlayerCharacter::Nick:   tex = &Texturemanager::getinstance().getNick(); break;
    case PlayerCharacter::Blue:   tex = &Texturemanager::getinstance().getPlayerBlue(); break;
    case PlayerCharacter::Green:  tex = &Texturemanager::getinstance().getPlayerGreen(); break;
    case PlayerCharacter::Red:    tex = &Texturemanager::getinstance().getPlayerRed(); break;
    case PlayerCharacter::Yellow: tex = &Texturemanager::getinstance().getPlayerYellow(); break;
    default:                      tex = &Texturemanager::getinstance().getNick(); break;
    }

    sprite.setTexture(*tex);

    int frameW, frameH;
    if (character == PlayerCharacter::Nick)
    {
        frameW = tex->getSize().x / 3;
        frameH = tex->getSize().y / 13;
    }
    else
    {
        frameW = tex->getSize().x;
        frameH = tex->getSize().y / 7;
    }

    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    sprite.setScale(width / (float)frameW, height / (float)frameH);
}

void Player::handleInput(float dt)
{
    if (!alive)
        return;

    float currentSpeed = PLAYER_SPEED;
    if (state.speedBoostActive)
        currentSpeed *= SPEED_BOOST_MULTIPLIER;

    velX = 0;

    bool moving = false;

    if (sf::Keyboard::isKeyPressed(keyLeft))
    {
        velX = -currentSpeed;
        facingright = false;
        moving = true;
    }
    if (sf::Keyboard::isKeyPressed(keyRight))
    {
        velX = currentSpeed;
        facingright = true;
        moving = true;
    }

    static bool jumpHeld[2] = { false, false };
    bool jumpNow = sf::Keyboard::isKeyPressed(keyJump);

    if (jumpNow && !jumpHeld[playerIndex] && onground)
    {
        velY = JUMP_FORCE;
        onground = false;
    }
    jumpHeld[playerIndex] = jumpNow;

    if (state.balloonActive)
    {
        velY = BALLOON_FLOAT_SPEED;
    }

    static bool fireHeld[2] = { false, false };
    bool fireNow = sf::Keyboard::isKeyPressed(keyFire);

    if (fireNow && !fireHeld[playerIndex] && fireCooldown <= 0)
    {
        wantFire = true;
        fireCooldown = 0.35f;
    }
    fireHeld[playerIndex] = fireNow;

    bool jumping = !onground;
    updateanimation(dt, moving, jumping);
}

void Player::update(float dt)
{
    if (!alive)
        return;

    if (fireCooldown > 0)
        fireCooldown -= dt;

    if (invincibletimer > 0)
        invincibletimer -= dt;

    updatepoweruptimers(dt);

    if (!state.balloonActive)
        applygravity(dt);

    x += velX;
    y += velY;

    if (x < 0) x = 0;
    if (x + width > WINDOW_WIDTH) x = WINDOW_WIDTH - width;
    if (y > WINDOW_HEIGHT + 100) loselife();

    updatehitbox();
}

void Player::collidingplatforms(Platform* platforms, int count)
{
    onground = false;

    for (int i = 0; i < count; i++)
    {
        sf::FloatRect pRect = platforms[i].getrect();

        if (hitbox.intersects(pRect))
        {
            float prevBottom = y + height - velY;
            if (velY >= 0 && prevBottom <= pRect.top + 8)
            {
                y = pRect.top - height;
                velY = 0;
                onground = true;
            }
            else if (velY < 0)
            {
                float prevTop = y - velY;
                if (prevTop >= pRect.top + pRect.height - 4)
                {
                    y = pRect.top + pRect.height;
                    velY = 1.0f;
                }
            }
        }
    }

    updatehitbox();
}

bool Player::tryFireSnowball()
{
    if (wantFire)
    {
        wantFire = false;
        return true;
    }
    return false;
}

void Player::resetfireflag() { wantFire = false; }
bool Player::wantstofire() const
{
    return wantFire;
}

void Player::loselife()
{
    if (invincibletimer > 0)
        return;
    state.lives--;
    invincibletimer = 2.5f;
    velX = 0;
    velY = -6.0f;
    if (state.lives <= 0)
    {
        alive = false;
    }
}
void Player::addlife()
{
    if (state.lives < MAX_LIVES)
        state.lives++;
}
void Player::addgems(int amount)
{
    state.gems = state.gems + amount;
}
void Player::addscore(int amount)
{
    state.score = state.score + amount;
}
void Player::applypowerup(Poweruptype type)
{
    switch (type)
    {
    case Poweruptype::speedboost:
        state.speedBoostActive = true;
        state.speedBoostTimer = SPEED_BOOST_DURATION;
        break;
    case Poweruptype::snowballpower:
        state.snowballPowerActive = true;
        snowballSpeedMult = 1.5f;
        break;
    case Poweruptype::increaseindistance:
        state.distanceBoostActive = true;
        snowballDistMult = 2.0f;
        break;
    case Poweruptype::ballonmode:
        state.balloonActive = true;
        state.balloonTimer = BALLOON_DURATION;
        break;
    case Poweruptype::extralife:
        addlife();
        break;
    case Poweruptype::Gem:
        addgems(10);
        break;
    default:
        break;
    }
}
void Player::clearlevelpowerups()
{
    state.snowballPowerActive = false;
    state.distanceBoostActive = false;
    snowballSpeedMult = 1.0f;
    snowballDistMult = 1.0f;
}
PlayerState& Player::getstate()
{
    return state;
}
int Player::getplayerindex() const
{
    return playerIndex;
}
PlayerCharacter Player::getcharacter() const
{
    return character;
}
float Player::getSnowballSpeedMult() const
{
    return snowballSpeedMult;
}
float Player::getSnowballDistMult() const
{
    return snowballDistMult;
}

bool Player::isinvincible() const { return invincibletimer > 0; }

void Player::respawn(float sx, float sy)
{
    x = sx;
    y = sy;
    velX = 0;
    velY = 0;
    if (state.lives > 0)
        alive = true;
    invincibletimer = 2.0f;
    updatehitbox();
}
void Player::updatepoweruptimers(float dt)
{
    if (state.speedBoostActive)
    {
        state.speedBoostTimer -= dt;
        if (state.speedBoostTimer <= 0)
        {
            state.speedBoostActive = false;
            state.speedBoostTimer = 0;
        }
    }
    if (state.balloonActive)
    {
        state.balloonTimer -= dt;
        if (state.balloonTimer <= 0)
        {
            state.balloonActive = false;
            state.balloonTimer = 0;
        }
    }
}
void Player::updateanimation(float dt, bool moving, bool jumping)
{
    anime_timer += dt;

    if (anime_timer >= ANIMATION_SPEED)
    {
        anime_timer = 0;

        if (jumping)
        {
            animRow = 1;
            animeframe = 0;
        }
        else if (moving)
        {
            animRow = 2;
            animeframe = (animeframe + 1) % 7;
        }
        else
        {
            animRow = 0;
            animeframe = (animeframe + 1) % 5;
        }
    }
    const sf::Texture* tex = sprite.getTexture();
    if (!tex || tex->getSize().x == 0) return;

    int frameW, frameH;
    if (character == PlayerCharacter::Nick)
    {
        frameW = tex->getSize().x / 3;
        frameH = tex->getSize().y / 13;
        int col = animeframe % 3;
        sprite.setTextureRect(sf::IntRect(col * frameW, animRow * frameH, frameW, frameH));
    }
    else
    {
        frameW = tex->getSize().x;
        frameH = tex->getSize().y / 7;
        sprite.setTextureRect(sf::IntRect(0, animRow * frameH, frameW, frameH));
    }
    if (facingright)
    {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    }
    else
    {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }
}
bool Player::isFacingRight() const
{
    return facingright;
}
void Player::draw(sf::RenderWindow& window)
{
    if (!alive)
        return;
    bool visible = true;
    if (invincibletimer > 0)
    {
        int flash = (int)(invincibletimer * 10) % 2;
        visible = (flash == 0);
    }

    if (visible)
    {
        drawPlayer(window);
    }
    drawhitbox(window, sf::Color::Green);
}
void Player::drawPlayer(sf::RenderWindow& window)
{
    if (facingright)
        sprite.setPosition(x, y);
    else
        sprite.setPosition(x + width, y);

    window.draw(sprite);
}