#pragma once
#include <SFML/Graphics.hpp>
#include "LeaderboardScreen.h"
#include <string>
#include "GameState.h"
#include "Constants.h"
#include "TextureManager.h"
#include "FileManager.h"
#include "Level.h"
#include "Player.h"
#include "Botom.h"
#include "FlyingFooga.h"
#include "Tornado.h"
#include "Mogera.h"
#include "Gamakichi.h"
#include "Snowball.h"
#include "Projectile.h"
#include "PowerUp.h"

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    sf::RenderWindow window;
    sf::Font font;
    GameScreen screen;

    bool multiplayerMode;
    int playerCount;
    int currentLevelNumber;
    int bonusLevels[2];
    bool levelIsBonusMap[TOTAL_LEVELS + 1];

    Player players[MAX_PLAYERS];
    int activePlayerCount;

    Botom       botoms[MAX_ENEMIES];
    FlyingFooga foogas[MAX_ENEMIES];
    Tornado     tornados[MAX_ENEMIES];
    Mogera      mogera;
    Mogerachild mogeraChildren[8];
    Gamakichi   gamakichi;
    bool        bossPresent;
    bool        isMogeraLevel;
    bool        isGamakichiLevel;

    int activeBottoms;
    int activeFoogas;
    int activeTornados;
    int activeMogeraChildren;

    Snowball    snowballs[MAX_PLAYERS][MAX_SNOWBALLS];
    Projectile  projectiles[MAX_PROJECTILES];
    Powerup     powerups[MAX_POWERUPS];

    Level       level;

    PlayerCharacter selectedChars[MAX_PLAYERS];

    sf::RectangleShape hudBg;
    sf::Text           scoreText[MAX_PLAYERS];
    sf::Text           livesText[MAX_PLAYERS];
    sf::Text           gemsText;
    sf::Text           levelText;
    sf::Text           powerupText[MAX_PLAYERS];

    sf::RectangleShape bossBarBg;
    sf::RectangleShape bossBarFill;
    sf::Text           bossBarLabel;

    sf::RectangleShape leaderboardBtn;
    sf::Text           leaderboardBtnText;

    char loginUsername[64];
    char loginPassword[64];
    char signupUsername[64];
    char signupPassword[64];
    int  loginField;
    int  signupField;
    bool loginError;
    bool signupError;
    std::string loginErrorMsg;
    std::string signupErrorMsg;

    sf::RectangleShape inputBoxes[4];
    sf::Text           inputTexts[4];
    sf::Text           inputLabels[4];
    sf::Text           titleText;
    sf::Text           promptText;
    sf::RectangleShape loginBtn;
    sf::Text           loginBtnText;
    sf::RectangleShape signupBtn;
    sf::Text           signupBtnText;
    sf::RectangleShape switchBtn;
    sf::Text           switchBtnText;

    int charSelectIndex[MAX_PLAYERS];
    int charSelectStep;
    sf::RectangleShape charCards[5];
    sf::Text           charNames[5];
    sf::Text           charSelectTitle;
    sf::Text           charSelectPrompt;
    sf::RectangleShape charSelectConfirm;
    sf::Text           charSelectConfirmText;
    sf::RectangleShape modeBtn[2];
    sf::Text           modeBtnText[2];

    bool pauseOnShop;
    int shopSelection;
    sf::RectangleShape shopBg;
    sf::Text           shopTitle;
    sf::RectangleShape shopItems[5];
    sf::Text           shopItemText[5];
    sf::Text           shopGemsText;
    sf::RectangleShape shopCloseBtn;
    sf::Text           shopCloseBtnText;

    sf::RectangleShape pauseBg;
    sf::Text           pauseTitle;
    sf::RectangleShape pauseButtons[4];
    sf::Text           pauseButtonText[4];
    int pauseSelection;

    sf::Text gameOverTitle;
    sf::Text gameOverScore;
    sf::Text gameOverPrompt;
    sf::RectangleShape gameOverRetry;
    sf::Text           gameOverRetryText;
    sf::RectangleShape gameOverMenu;
    sf::Text           gameOverMenuText;

    sf::Text levelCompleteTitle;
    sf::Text levelCompleteScore;
    sf::Text levelCompletePrompt;
    float levelCompleteTimer;

    sf::Text bossDefeatedTitle;
    sf::Text bossDefeatedReward;
    float bossDefeatedTimer;

    bool bonusRainActive;
    float bonusRainTimer;
    float bonusSpawnTimer;
    Powerup bonusGems[30];
    int bonusGemCount;

    bool starChoiceActive;
    Poweruptype starChoices[3];
    int starChoiceSelected;
    sf::RectangleShape starChoiceBoxes[3];
    sf::Text           starChoiceTexts[3];
    sf::Text           starChoiceTitle;

    float splashTimer;
    sf::Text splashTitle;
    sf::Text splashSubtitle;
    sf::RectangleShape splashBg;

    Leaderboardscreen leaderboardScreen;

    void processEvents();
    void update(float dt);
    void render();

    void handleLoginEvent(sf::Event& e);
    void handleSignupEvent(sf::Event& e);
    void handleCharSelectEvent(sf::Event& e);
    void handlePlayingEvent(sf::Event& e);
    void handlePausedEvent(sf::Event& e);
    void handleShopEvent(sf::Event& e);
    void handleGameOverEvent(sf::Event& e);
    void handleLevelCompleteEvent(sf::Event& e);
    void handleBossDefeatedEvent(sf::Event& e);
    void handleLeaderboardEvent(sf::Event& e);
    void handleStarChoiceEvent(sf::Event& e);

    void updatePlaying(float dt);
    void updateBonusRain(float dt);

    void renderSplash();
    void renderLogin();
    void renderSignup();
    void renderCharSelect();
    void renderPlaying();
    void renderPaused();
    void renderShop();
    void renderGameOver();
    void renderLevelComplete();
    void renderBossDefeated();
    void renderHUD();
    void renderBossBar();
    void renderStarChoice();
    void renderBonusRain();

    void startGame();
    void loadLevel(int num);
    void spawnEnemiesForLevel();
    void checkSnowballEnemyCollisions();
    void checkPlayerEnemyCollisions();
    void checkPlayerPowerUpCollisions();
    void checkRollingSnowballCollisions();
    void fireSnowballForPlayer(int pi);
    void fireEnemyProjectiles();
    void trySpawnMogeraChild();
    void tryFireGamakichi();
    void checkLevelComplete();
    void onLevelComplete();
    void onGameOver();
    void applyChainScore(int chain, int pi);
    void spawnPowerUp(float px, float py, Poweruptype t);
    void pickBonusLevels();
    bool allEnemiesDead();
    int  getTotalEnemyCount();
    void saveCurrentProgress();
    void buyShopItem(int idx);
    void initUI();
    void initSplash();
    void initLoginUI();
    void initSignupUI();
    void initCharSelectUI();
    void initPauseUI();
    void initShopUI();
    void initGameOverUI();
    void initLevelCompleteUI();
    void initBossDefeatedUI();
    void initHUD();
    void updateHUDText();
    void appendChar(char* buf, int maxLen, char c);
    void deleteChar(char* buf);
    std::string maskString(const char* buf);
};