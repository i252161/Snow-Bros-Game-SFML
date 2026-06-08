#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <sstream>


static std::string inttostr(int v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

static sf::Color darken(sf::Color c, int amt)
{
    int r = c.r - amt; if (r < 0) r = 0;
    int g = c.g - amt; if (g < 0) g = 0;
    int b = c.b - amt; if (b < 0) b = 0;
    return sf::Color(r, g, b);
}

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snow Bros", sf::Style::Close)
{
    srand((unsigned)time(0));
    window.setFramerateLimit(FPS);

    font.loadFromFile("Assets/Fonts/arial.ttf");

    screen = GameScreen::Splash;
    multiplayerMode = false;
    playerCount = 1;
    currentLevelNumber = 1;
    activePlayerCount = 1;
    bossPresent = false;
    isMogeraLevel = false;
    isGamakichiLevel = false;
    activeBottoms = 0;
    activeFoogas = 0;
    activeTornados = 0;
    activeMogeraChildren = 0;
    pauseOnShop = false;
    shopSelection = 0;
    pauseSelection = 0;
    levelCompleteTimer = 0;
    bossDefeatedTimer = 0;
    bonusRainActive = false;
    bonusRainTimer = 0;
    bonusSpawnTimer = 0;
    bonusGemCount = 0;
    starChoiceActive = false;
    starChoiceSelected = 0;
    splashTimer = 0;

    loginField = 0;
    signupField = 0;
    loginError = false;
    signupError = false;
    memset(loginUsername, 0, sizeof(loginUsername));
    memset(loginPassword, 0, sizeof(loginPassword));
    memset(signupUsername, 0, sizeof(signupUsername));
    memset(signupPassword, 0, sizeof(signupPassword));

    charSelectStep = 0;
    charSelectIndex[0] = 0;
    charSelectIndex[1] = 0;
    selectedChars[0] = PlayerCharacter::Nick;
    selectedChars[1] = PlayerCharacter::Blue;

    memset(levelIsBonusMap, 0, sizeof(levelIsBonusMap));

    Texturemanager::getinstance().loadAll();

    initUI();
}

Game::~Game() {}

void Game::run()
{
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    sf::Event e;
    while (window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
            window.close();

        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F1)
            Entity::toggleHitboxDebug();

        switch (screen)
        {
        case GameScreen::Login:        handleLoginEvent(e);        break;
        case GameScreen::Signup:       handleSignupEvent(e);       break;
        case GameScreen::CharacterSelect: handleCharSelectEvent(e); break;
        case GameScreen::Playing:      handlePlayingEvent(e);      break;
        case GameScreen::Paused:       handlePausedEvent(e);       break;
        case GameScreen::Shop:         handleShopEvent(e);         break;
        case GameScreen::GameOver:     handleGameOverEvent(e);     break;
        case GameScreen::LevelComplete: handleLevelCompleteEvent(e); break;
        case GameScreen::BossDefeated: handleBossDefeatedEvent(e); break;
        case GameScreen::Leaderboard: handleLeaderboardEvent(e); break;
        default: break;
        }

        if (starChoiceActive && screen == GameScreen::Playing)
            handleStarChoiceEvent(e);
    }
}

void Game::update(float dt)
{
    switch (screen)
    {
    case GameScreen::Splash:
        splashTimer += dt;
        if (splashTimer > 2.5f)
            screen = GameScreen::Login;
        break;
    case GameScreen::Playing:
        updatePlaying(dt);
        break;
    case GameScreen::LevelComplete:
        levelCompleteTimer += dt;
        if (levelCompleteTimer > 3.0f)
        {
            currentLevelNumber++;
            if (currentLevelNumber > TOTAL_LEVELS)
            {
                onGameOver();
            }
            else
            {
                loadLevel(currentLevelNumber);
                screen = GameScreen::Playing;
            }
        }
        break;
    case GameScreen::BossDefeated:
        bossDefeatedTimer += dt;
        if (bossDefeatedTimer > 3.5f)
        {
            currentLevelNumber++;
            if (currentLevelNumber > TOTAL_LEVELS)
                onGameOver();
            else
            {
                loadLevel(currentLevelNumber);
                screen = GameScreen::Playing;
            }
        }
        break;
    default:
        break;
    }
}

void Game::updatePlaying(float dt)
{
    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        if (!players[pi].isalive()) continue;
        players[pi].handleInput(dt);
        players[pi].update(dt);
        players[pi].collidingplatforms(level.getplatforms(), level.getplatformcount());
    }

    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        for (int si = 0; si < MAX_SNOWBALLS; si++)
        {
            if (!snowballs[pi][si].isalive()) continue;
            snowballs[pi][si].update(dt);
            snowballs[pi][si].collidingplatforms(level.getplatforms(), level.getplatformcount());
        }
        if (players[pi].isalive() && players[pi].tryFireSnowball())
            fireSnowballForPlayer(pi);
    }

    for (int i = 0; i < activeBottoms; i++)
    {
        if (!botoms[i].isalive()) continue;
        botoms[i].update(dt);
        botoms[i].collidingplatforms(level.getplatforms(), level.getplatformcount());
    }

    for (int i = 0; i < activeFoogas; i++)
    {
        if (!foogas[i].isalive()) continue;
        foogas[i].update(dt);
        foogas[i].collidingplatforms(level.getplatforms(), level.getplatformcount());
    }

    for (int i = 0; i < activeTornados; i++)
    {
        if (!tornados[i].isalive()) continue;
        if (activePlayerCount > 0 && players[0].isalive())
            tornados[i].setpositionplayer(players[0].getX(), players[0].getY());
        tornados[i].update(dt);
        tornados[i].collidingplatforms(level.getplatforms(), level.getplatformcount());
    }

    if (bossPresent)
    {
        if (isMogeraLevel && mogera.isalive())
        {
            mogera.update(dt);
            trySpawnMogeraChild();
        }
        if (isGamakichiLevel && gamakichi.isalive())
        {
            gamakichi.update(dt);
            tryFireGamakichi();
        }

        for (int i = 0; i < activeMogeraChildren; i++)
        {
            if (!mogeraChildren[i].isalive()) continue;
            mogeraChildren[i].update(dt);
            mogeraChildren[i].collidingplatforms(level.getplatforms(), level.getplatformcount());
        }
    }

    fireEnemyProjectiles();

    for (int i = 0; i < MAX_PROJECTILES; i++)
        projectiles[i].update(dt);

    for (int i = 0; i < MAX_POWERUPS; i++)
        powerups[i].update(dt);

    checkSnowballEnemyCollisions();
    checkRollingSnowballCollisions();
    checkPlayerEnemyCollisions();
    checkPlayerPowerUpCollisions();

    if (bonusRainActive)
        updateBonusRain(dt);

    updateHUDText();
    checkLevelComplete();
}

void Game::updateBonusRain(float dt)
{
    bonusRainTimer += dt;
    bonusSpawnTimer += dt;

    if (bonusSpawnTimer > 0.3f && bonusGemCount < 30)
    {
        bonusSpawnTimer = 0;
        bonusGems[bonusGemCount].spawn(
            50.f + rand() % 700,
            -20.f,
            Poweruptype::Gem
        );
        bonusGemCount++;
    }

    for (int i = 0; i < bonusGemCount; i++)
    {
        if (!bonusGems[i].isalive()) continue;
        bonusGems[i].update(dt);

        for (int pi = 0; pi < activePlayerCount; pi++)
        {
            if (!players[pi].isalive()) continue;
            if (bonusGems[i].gethitbox().intersects(players[pi].gethitbox()))
            {
                bonusGems[i].collect();
                players[pi].addgems(10);
                players[pi].addscore(1000);
            }
        }
    }

    if (bonusRainTimer > 8.0f)
    {
        bonusRainActive = false;
        bonusRainTimer = 0;
        bonusGemCount = 0;
    }
}

void Game::render()
{
    window.clear(sf::Color(10, 10, 30));

    switch (screen)
    {
    case GameScreen::Splash:        renderSplash();        break;
    case GameScreen::Login:         renderLogin();         break;
    case GameScreen::Signup:        renderSignup();        break;
    case GameScreen::CharacterSelect: renderCharSelect();  break;
    case GameScreen::Playing:
        renderPlaying();
        if (starChoiceActive) renderStarChoice();
        if (bonusRainActive)  renderBonusRain();
        break;
    case GameScreen::Paused:        renderPlaying(); renderPaused(); break;
    case GameScreen::Shop:          renderPlaying(); renderShop();   break;
    case GameScreen::GameOver:      renderGameOver();      break;
    case GameScreen::LevelComplete: renderLevelComplete(); break;
    case GameScreen::BossDefeated:  renderBossDefeated();  break;
    case GameScreen::Leaderboard: leaderboardScreen.render(window); break;
    }

    window.display();
}

void Game::renderSplash()
{
    window.draw(splashBg);
    window.draw(splashTitle);
    window.draw(splashSubtitle);
}

void Game::renderLogin()
{
    window.draw(splashBg);
    window.draw(titleText);
    for (int i = 0; i < 2; i++)
    {
        window.draw(inputBoxes[i]);
        window.draw(inputLabels[i]);
        window.draw(inputTexts[i]);
    }
    window.draw(loginBtn);
    window.draw(loginBtnText);
    window.draw(switchBtn);
    window.draw(switchBtnText);
    if (loginError)
    {
        sf::Text err;
        err.setFont(font);
        err.setString(loginErrorMsg);
        err.setCharacterSize(16);
        err.setFillColor(sf::Color(255, 80, 80));
        err.setPosition(200, 420);
        window.draw(err);

    }
    window.draw(leaderboardBtn);
    window.draw(leaderboardBtnText);
}

void Game::renderSignup()
{
    window.draw(splashBg);
    window.draw(titleText);
    for (int i = 0; i < 2; i++)
    {
        window.draw(inputBoxes[i]);
        window.draw(inputLabels[i]);
        window.draw(inputTexts[i]);
    }
    window.draw(signupBtn);
    window.draw(signupBtnText);
    window.draw(switchBtn);
    window.draw(switchBtnText);
    if (signupError)
    {
        sf::Text err;
        err.setFont(font);
        err.setString(signupErrorMsg);
        err.setCharacterSize(16);
        err.setFillColor(sf::Color(255, 80, 80));
        err.setPosition(200, 420);
        window.draw(err);
    }
}

void Game::renderCharSelect()
{
    window.draw(splashBg);
    window.draw(charSelectTitle);
    window.draw(charSelectPrompt);

    if (charSelectStep == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            window.draw(modeBtn[i]);
            window.draw(modeBtnText[i]);
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            window.draw(charCards[i]);
            window.draw(charNames[i]);
        }
        window.draw(charSelectConfirm);
        window.draw(charSelectConfirmText);
    }
}

void Game::renderPlaying()
{
    level.draw(window);

    for (int i = 0; i < MAX_POWERUPS; i++)
        powerups[i].draw(window);

    for (int i = 0; i < activeBottoms; i++)
        botoms[i].draw(window);
    for (int i = 0; i < activeFoogas; i++)
        foogas[i].draw(window);
    for (int i = 0; i < activeTornados; i++)
        tornados[i].draw(window);

    if (bossPresent)
    {
        if (isMogeraLevel)  mogera.draw(window);
        if (isGamakichiLevel) gamakichi.draw(window);
        for (int i = 0; i < activeMogeraChildren; i++)
            mogeraChildren[i].draw(window);
    }

    for (int i = 0; i < MAX_PROJECTILES; i++)
        projectiles[i].draw(window);

    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        for (int si = 0; si < MAX_SNOWBALLS; si++)
            snowballs[pi][si].draw(window);
        players[pi].draw(window);
    }

    renderHUD();
    if (bossPresent) renderBossBar();
}

void Game::renderHUD()
{
    window.draw(hudBg);

    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        window.draw(scoreText[pi]);
        window.draw(livesText[pi]);
        window.draw(powerupText[pi]);
    }
    window.draw(gemsText);
    window.draw(levelText);
}

void Game::renderBossBar()
{
    int bossHp = 0, bossMax = 1;
    if (isMogeraLevel && mogera.isalive())
    {
        bossHp = mogera.gethealth();
        bossMax = mogera.getmaxhealth();
    }
    else if (isGamakichiLevel && gamakichi.isalive())
    {
        bossHp = gamakichi.gethealth();
        bossMax = gamakichi.getmaxhealth();
    }
    else return;

    float pct = (float)bossHp / (float)bossMax;
    bossBarFill.setSize(sf::Vector2f(240.f * pct, 14.f));
    if (pct < 0.3f)      bossBarFill.setFillColor(sf::Color(220, 60, 40));
    else if (pct < 0.6f) bossBarFill.setFillColor(sf::Color(220, 180, 40));
    else                 bossBarFill.setFillColor(sf::Color(80, 220, 80));

    window.draw(bossBarBg);
    window.draw(bossBarFill);
    window.draw(bossBarLabel);
}

void Game::renderStarChoice()
{
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    window.draw(starChoiceTitle);
    for (int i = 0; i < 3; i++)
    {
        window.draw(starChoiceBoxes[i]);
        window.draw(starChoiceTexts[i]);
    }
}

void Game::renderBonusRain()
{
    for (int i = 0; i < bonusGemCount; i++)
        bonusGems[i].draw(window);
}

void Game::renderPaused()
{
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    window.draw(pauseBg);
    window.draw(pauseTitle);
    for (int i = 0; i < 4; i++)
    {
        window.draw(pauseButtons[i]);
        window.draw(pauseButtonText[i]);
    }
}

void Game::renderShop()
{
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    window.draw(shopBg);
    window.draw(shopTitle);
    window.draw(shopGemsText);
    for (int i = 0; i < 5; i++)
    {
        window.draw(shopItems[i]);
        window.draw(shopItemText[i]);
    }
    window.draw(shopCloseBtn);
    window.draw(shopCloseBtnText);
}

void Game::renderGameOver()
{
    window.draw(splashBg);
    window.draw(gameOverTitle);
    window.draw(gameOverScore);
    window.draw(gameOverPrompt);
    window.draw(gameOverRetry);
    window.draw(gameOverRetryText);
    window.draw(gameOverMenu);
    window.draw(gameOverMenuText);
}

void Game::renderLevelComplete()
{
    level.draw(window);
    window.draw(levelCompleteTitle);
    window.draw(levelCompleteScore);
    window.draw(levelCompletePrompt);
}

void Game::renderBossDefeated()
{
    level.draw(window);
    window.draw(bossDefeatedTitle);
    window.draw(bossDefeatedReward);
}
void Game::handleLoginEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);

        if (inputBoxes[0].getGlobalBounds().contains(mp)) loginField = 0;
        if (inputBoxes[1].getGlobalBounds().contains(mp)) loginField = 1;

        if (loginBtn.getGlobalBounds().contains(mp))
        {
            if (FileManager::getinstance().loginUser(loginUsername, loginPassword))
            {
                loginError = false;
                screen = GameScreen::CharacterSelect;
                charSelectStep = 0;
                initCharSelectUI();
            }
            else
            {
                loginError = true;
                loginErrorMsg = "Invalid username or password.";
            }
        }

        if (switchBtn.getGlobalBounds().contains(mp))
        {
            screen = GameScreen::Signup;
            loginError = false;
            memset(loginUsername, 0, sizeof(loginUsername));
            memset(loginPassword, 0, sizeof(loginPassword));
            initSignupUI();
        }

        if (leaderboardBtn.getGlobalBounds().contains(mp))
        {
            leaderboardScreen.refresh();
            screen = GameScreen::Leaderboard;
        }
    }

    if (e.type == sf::Event::TextEntered)
    {
        char c = (char)e.text.unicode;
        if (c == '\b')
        {
            if (loginField == 0) deleteChar(loginUsername);
            else                 deleteChar(loginPassword);
        }
        else if (c >= 32 && c < 127)
        {
            if (loginField == 0) appendChar(loginUsername, 63, c);
            else                 appendChar(loginPassword, 63, c);
        }

        inputTexts[0].setString(loginUsername);
        inputTexts[1].setString(maskString(loginPassword));
    }
}

void Game::handleSignupEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);

        if (inputBoxes[0].getGlobalBounds().contains(mp)) signupField = 0;
        if (inputBoxes[1].getGlobalBounds().contains(mp)) signupField = 1;

        if (signupBtn.getGlobalBounds().contains(mp))
        {
            if (strlen(signupUsername) < 3)
            {
                signupError = true;
                signupErrorMsg = "Username must be at least 3 characters.";
            }
            else if (strlen(signupPassword) < 4)
            {
                signupError = true;
                signupErrorMsg = "Password must be at least 4 characters.";
            }
            else if (FileManager::getinstance().registerUser(signupUsername, signupPassword))
            {
                FileManager::getinstance().loginUser(signupUsername, signupPassword);
                signupError = false;
                screen = GameScreen::CharacterSelect;
                charSelectStep = 0;
                initCharSelectUI();
            }
            else
            {
                signupError = true;
                signupErrorMsg = "Username already taken.";
            }
        }

        if (switchBtn.getGlobalBounds().contains(mp))
        {
            screen = GameScreen::Login;
            signupError = false;
            memset(signupUsername, 0, sizeof(signupUsername));
            memset(signupPassword, 0, sizeof(signupPassword));
            initLoginUI();
        }
    }

    if (e.type == sf::Event::TextEntered)
    {
        char c = (char)e.text.unicode;
        if (c == '\b')
        {
            if (signupField == 0) deleteChar(signupUsername);
            else                  deleteChar(signupPassword);
        }
        else if (c >= 32 && c < 127)
        {
            if (signupField == 0) appendChar(signupUsername, 63, c);
            else                  appendChar(signupPassword, 63, c);
        }

        inputTexts[0].setString(signupUsername);
        inputTexts[1].setString(maskString(signupPassword));
    }
}

void Game::handleCharSelectEvent(sf::Event& e)
{
    if (e.type != sf::Event::MouseButtonPressed) return;
    sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);

    if (charSelectStep == 0)
    {
        if (modeBtn[0].getGlobalBounds().contains(mp))
        {
            multiplayerMode = false;
            activePlayerCount = 1;
            charSelectStep = 1;
            initCharSelectUI();
        }
        else if (modeBtn[1].getGlobalBounds().contains(mp))
        {
            multiplayerMode = true;
            activePlayerCount = 2;
            charSelectStep = 1;
            initCharSelectUI();
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (charCards[i].getGlobalBounds().contains(mp))
            {
                int pi = multiplayerMode ? (charSelectStep - 1) : 0;
                charSelectIndex[pi] = i;

                sf::Color highlight(80, 140, 220);
                for (int j = 0; j < 5; j++)
                    charCards[j].setFillColor(sf::Color(40, 40, 80));
                charCards[i].setFillColor(highlight);
            }
        }

        if (charSelectConfirm.getGlobalBounds().contains(mp))
        {
            int pi = multiplayerMode ? (charSelectStep - 1) : 0;
            PlayerCharacter chars[5] = {
                PlayerCharacter::Nick,
                PlayerCharacter::Blue,
                PlayerCharacter::Green,
                PlayerCharacter::Red,
                PlayerCharacter::Yellow
            };
            selectedChars[pi] = chars[charSelectIndex[pi]];

            if (multiplayerMode && charSelectStep == 1)
            {
                charSelectStep = 2;
                charSelectIndex[1] = 1;
                for (int j = 0; j < 5; j++)
                    charCards[j].setFillColor(sf::Color(40, 40, 80));
                charCards[1].setFillColor(sf::Color(80, 140, 220));

                charSelectTitle.setString("Player 2 - Choose Character");
            }
            else
            {
                startGame();
            }
        }
    }
}

void Game::handlePlayingEvent(sf::Event& e)
{
    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Escape || e.key.code == sf::Keyboard::P)
        {
            screen = GameScreen::Paused;
            pauseSelection = 0;
        }
    }
}

void Game::handlePausedEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);

        if (pauseButtons[0].getGlobalBounds().contains(mp))
            screen = GameScreen::Playing;

        else if (pauseButtons[1].getGlobalBounds().contains(mp))
        {
            screen = GameScreen::Shop;
            shopGemsText.setString("Your Gems: " + inttostr(players[0].getstate().gems));
        }

        else if (pauseButtons[2].getGlobalBounds().contains(mp))
        {
            saveCurrentProgress();
            screen = GameScreen::Playing;
        }

        else if (pauseButtons[3].getGlobalBounds().contains(mp))
        {
            FileManager::getinstance().logout();
            screen = GameScreen::Login;
            memset(loginUsername, 0, sizeof(loginUsername));
            memset(loginPassword, 0, sizeof(loginPassword));
            initLoginUI();
        }
    }

    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
        screen = GameScreen::Playing;
}

void Game::handleShopEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);

        for (int i = 0; i < 5; i++)
        {
            if (shopItems[i].getGlobalBounds().contains(mp))
                buyShopItem(i);
        }

        if (shopCloseBtn.getGlobalBounds().contains(mp))
            screen = GameScreen::Paused;
    }
}

void Game::handleGameOverEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);

        if (gameOverRetry.getGlobalBounds().contains(mp))
        {
            currentLevelNumber = 1;
            startGame();
        }

        if (gameOverMenu.getGlobalBounds().contains(mp))
        {
            screen = GameScreen::Login;
            memset(loginUsername, 0, sizeof(loginUsername));
            memset(loginPassword, 0, sizeof(loginPassword));
            initLoginUI();
        }
    }
}

void Game::handleLevelCompleteEvent(sf::Event& e) {}
void Game::handleBossDefeatedEvent(sf::Event& e) {}

void Game::handleStarChoiceEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);
        for (int i = 0; i < 3; i++)
        {
            if (starChoiceBoxes[i].getGlobalBounds().contains(mp))
            {
                players[0].applypowerup(starChoices[i]);
                starChoiceActive = false;
                break;
            }
        }
    }
}

void Game::startGame()
{
    pickBonusLevels();
    currentLevelNumber = 1;

    players[0].init(150, 400, 0, selectedChars[0]);
    players[0].setControls(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::Space);
    players[0].getstate().lives = START_LIVES;
    players[0].getstate().gems = 0;

    if (activePlayerCount > 1)
    {
        players[1].init(600, 400, 1, selectedChars[1]);
        players[1].setControls(
            sf::Keyboard::Left, sf::Keyboard::Right,
            sf::Keyboard::Up, sf::Keyboard::Return);
        players[1].getstate().lives = START_LIVES;
        players[1].getstate().gems = 0;
    }

    loadLevel(currentLevelNumber);
    screen = GameScreen::Playing;
}

void Game::loadLevel(int num)
{
    bool isbonus = levelIsBonusMap[num];
    level.load(num, isbonus);
    currentLevelNumber = num;

    activeBottoms = 0;
    activeFoogas = 0;
    activeTornados = 0;
    activeMogeraChildren = 0;
    bossPresent = false;
    isMogeraLevel = false;
    isGamakichiLevel = false;
    bonusRainActive = false;
    bonusGemCount = 0;
    starChoiceActive = false;

    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        players[pi].clearlevelpowerups();
        for (int si = 0; si < MAX_SNOWBALLS; si++)
            snowballs[pi][si].setalive(false);
    }

    for (int i = 0; i < MAX_PROJECTILES; i++)
        projectiles[i].setalive(false);
    for (int i = 0; i < MAX_POWERUPS; i++)
        powerups[i].setalive(false);

    spawnEnemiesForLevel();

    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        if (players[pi].getstate().lives > 0)
            players[pi].respawn(150 + pi * 400, 400);
    }

    levelText.setString("Level " + inttostr(num) + "/" + inttostr(TOTAL_LEVELS));
}

void Game::spawnEnemiesForLevel()
{
    LevelConfig& cfg = level.getconfig();

    for (int i = 0; i < cfg.enemyCount; i++)
    {
        EnemySpawnInfo& inf = cfg.enemies[i];
        float ex = inf.x;
        float ey = inf.y;
        int   v = inf.variant;

        switch (inf.enemyKind)
        {
        case 0:
            if (activeBottoms < MAX_ENEMIES)
            {
                botoms[activeBottoms].setvariant(v);
                botoms[activeBottoms].init(ex, ey);
                activeBottoms++;
            }
            break;
        case 1:
            if (activeFoogas < MAX_ENEMIES)
            {
                foogas[activeFoogas].setvariant(v);
                foogas[activeFoogas].init(ex, ey);
                activeFoogas++;
            }
            break;
        case 2:
            if (activeTornados < MAX_ENEMIES)
            {
                tornados[activeTornados].setvariant(v);
                tornados[activeTornados].init(ex, ey);
                activeTornados++;
            }
            break;
        case 3:
            mogera.init(ex, ey);
            bossPresent = true;
            isMogeraLevel = true;
            break;
        case 4:
            gamakichi.init(ex, ey);
            bossPresent = true;
            isGamakichiLevel = true;
            break;
        }
    }
}
void Game::fireSnowballForPlayer(int pi)
{
    for (int si = 0; si < MAX_SNOWBALLS; si++)
    {
        if (!snowballs[pi][si].isalive())
        {
            float sx = players[pi].getX();
            float sy = players[pi].getY() + players[pi].getheight() * 0.3f;
            bool facingR = players[pi].isFacingRight();
            float speedMult = players[pi].getSnowballSpeedMult();
            snowballs[pi][si].setownerindex(pi);
            snowballs[pi][si].launch(sx, sy, facingR, speedMult);
            break;
        }
    }
}

void Game::checkSnowballEnemyCollisions()
{
    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        for (int si = 0; si < MAX_SNOWBALLS; si++)
        {
            if (!snowballs[pi][si].isalive() || snowballs[pi][si].isRolling()) continue;

            sf::FloatRect sRect = snowballs[pi][si].gethitbox();

            for (int i = 0; i < activeBottoms; i++)
            {
                if (!botoms[i].isalive() || botoms[i].isencased()) continue;
                if (sRect.intersects(botoms[i].gethitbox()))
                {
                    botoms[i].hitbysnowball();
                    snowballs[pi][si].setHitEnemy(true);
                    if (botoms[i].isencased())
                        snowballs[pi][si].startrolling();
                    else
                        snowballs[pi][si].setalive(false);
                }
            }

            for (int i = 0; i < activeFoogas; i++)
            {
                if (!foogas[i].isalive() || foogas[i].isencased()) continue;
                if (sRect.intersects(foogas[i].gethitbox()))
                {
                    foogas[i].hitbysnowball();
                    snowballs[pi][si].setHitEnemy(true);
                    if (foogas[i].isencased())
                        snowballs[pi][si].startrolling();
                    else
                        snowballs[pi][si].setalive(false);
                }
            }

            for (int i = 0; i < activeTornados; i++)
            {
                if (!tornados[i].isalive() || tornados[i].isencased()) continue;
                if (sRect.intersects(tornados[i].gethitbox()))
                {
                    tornados[i].hitbysnowball();
                    snowballs[pi][si].setHitEnemy(true);
                    if (tornados[i].isencased())
                        snowballs[pi][si].startrolling();
                    else
                        snowballs[pi][si].setalive(false);
                }
            }

            for (int i = 0; i < activeMogeraChildren; i++)
            {
                if (!mogeraChildren[i].isalive() || mogeraChildren[i].isencased()) continue;
                if (sRect.intersects(mogeraChildren[i].gethitbox()))
                {
                    mogeraChildren[i].hitbysnowball();
                    snowballs[pi][si].setHitEnemy(true);
                    if (mogeraChildren[i].isencased())
                        snowballs[pi][si].startrolling();
                    else
                        snowballs[pi][si].setalive(false);
                }
            }

            if (bossPresent)
            {
                if (isMogeraLevel && mogera.isalive() && sRect.intersects(mogera.gethitbox()))
                {
                    mogera.hitbysnowball();
                    snowballs[pi][si].setalive(false);
                    if (!mogera.isalive())
                    {
                        players[pi].addscore(MOGERA_SCORE);
                        players[pi].addgems(200);
                        bossDefeatedTitle.setString("MOGERA DEFEATED!");
                        bossDefeatedReward.setString("+ 200 Gems  + 5000 pts");
                        bossDefeatedTimer = 0;
                        screen = GameScreen::BossDefeated;
                    }
                }
                if (isGamakichiLevel && gamakichi.isalive() && sRect.intersects(gamakichi.gethitbox()))
                {
                    gamakichi.hitbysnowball();
                    snowballs[pi][si].setalive(false);
                    if (!gamakichi.isalive())
                    {
                        players[pi].addscore(GAMAKICHI_SCORE);
                        players[pi].addgems(500);
                        bossDefeatedTitle.setString("GAMAKICHI DEFEATED!");
                        bossDefeatedReward.setString("+ 500 Gems  + 10000 pts");
                        bossDefeatedTimer = 0;
                        screen = GameScreen::BossDefeated;
                    }
                }
            }
        }
    }
}void Game::checkRollingSnowballCollisions()
{
    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        for (int si = 0; si < MAX_SNOWBALLS; si++)
        {
            if (!snowballs[pi][si].isalive() || !snowballs[pi][si].isRolling()) continue;

            sf::FloatRect sRect = snowballs[pi][si].gethitbox();

            for (int i = 0; i < activeBottoms; i++)
            {
                if (!botoms[i].isalive()) continue;
                if (sRect.intersects(botoms[i].gethitbox()))
                {
                    if (!botoms[i].isencased())
                        botoms[i].hitbysnowball();
                    if (botoms[i].isencased())
                    {
                        botoms[i].startrolling();
                        applyChainScore(snowballs[pi][si].getkillchain(), pi);
                        snowballs[pi][si].addkill();
                        if (botoms[i].dropspickup())
                            spawnPowerUp(botoms[i].getX(), botoms[i].getY(), botoms[i].getdroptype());
                    }
                }
            }

            for (int i = 0; i < activeFoogas; i++)
            {
                if (!foogas[i].isalive()) continue;
                if (sRect.intersects(foogas[i].gethitbox()))
                {
                    if (!foogas[i].isencased())
                        foogas[i].hitbysnowball();
                    if (foogas[i].isencased())
                    {
                        foogas[i].startrolling();
                        applyChainScore(snowballs[pi][si].getkillchain(), pi);
                        snowballs[pi][si].addkill();
                        if (foogas[i].dropspickup())
                            spawnPowerUp(foogas[i].getX(), foogas[i].getY(), foogas[i].getdroptype());
                    }
                }
            }

            for (int i = 0; i < activeTornados; i++)
            {
                if (!tornados[i].isalive()) continue;
                if (sRect.intersects(tornados[i].gethitbox()))
                {
                    if (!tornados[i].isencased())
                        tornados[i].hitbysnowball();
                    if (tornados[i].isencased())
                    {
                        tornados[i].startrolling();
                        applyChainScore(snowballs[pi][si].getkillchain(), pi);
                        snowballs[pi][si].addkill();
                        if (tornados[i].dropspickup())
                            spawnPowerUp(tornados[i].getX(), tornados[i].getY(), tornados[i].getdroptype());
                    }
                }
            }

            for (int i = 0; i < activeMogeraChildren; i++)
            {
                if (!mogeraChildren[i].isalive()) continue;
                if (sRect.intersects(mogeraChildren[i].gethitbox()))
                {
                    if (!mogeraChildren[i].isencased())
                        mogeraChildren[i].hitbysnowball();
                    if (mogeraChildren[i].isencased())
                    {
                        mogeraChildren[i].startrolling();
                        applyChainScore(snowballs[pi][si].getkillchain(), pi);
                        snowballs[pi][si].addkill();
                    }
                }
            }
        }
    }
}
void Game::checkPlayerEnemyCollisions()
{
    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        if (!players[pi].isalive() || players[pi].isinvincible()) continue;

        sf::FloatRect pRect = players[pi].gethitbox();

        for (int i = 0; i < activeBottoms; i++)
        {
            if (!botoms[i].isalive() || botoms[i].isencased()) continue;
            if (pRect.intersects(botoms[i].gethitbox()))
                players[pi].loselife();
        }

        for (int i = 0; i < activeFoogas; i++)
        {
            if (!foogas[i].isalive() || foogas[i].isencased()) continue;
            if (pRect.intersects(foogas[i].gethitbox()))
                players[pi].loselife();
        }

        for (int i = 0; i < activeTornados; i++)
        {
            if (!tornados[i].isalive() || tornados[i].isencased()) continue;
            if (pRect.intersects(tornados[i].gethitbox()))
                players[pi].loselife();
        }

        for (int i = 0; i < activeMogeraChildren; i++)
        {
            if (!mogeraChildren[i].isalive()) continue;
            if (pRect.intersects(mogeraChildren[i].gethitbox()))
                players[pi].loselife();
        }

        if (bossPresent)
        {
            if (isMogeraLevel && mogera.isalive() && pRect.intersects(mogera.gethitbox()))
                players[pi].loselife();
            if (isGamakichiLevel && gamakichi.isalive() && pRect.intersects(gamakichi.gethitbox()))
                players[pi].loselife();
        }

        for (int i = 0; i < MAX_PROJECTILES; i++)
        {
            if (!projectiles[i].isalive()) continue;
            if (pRect.intersects(projectiles[i].gethitbox()))
            {
                players[pi].loselife();
                projectiles[i].setalive(false);
            }
        }
    }

    bool anyAlive = false;
    for (int pi = 0; pi < activePlayerCount; pi++)
        if (players[pi].isalive()) anyAlive = true;

    if (!anyAlive)
        onGameOver();
}

void Game::checkPlayerPowerUpCollisions()
{
    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        if (!players[pi].isalive()) continue;
        sf::FloatRect pRect = players[pi].gethitbox();

        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (!powerups[i].isalive() || powerups[i].iscollected()) continue;
            if (pRect.intersects(powerups[i].gethitbox()))
            {
                Poweruptype t = powerups[i].gettype();
                powerups[i].collect();

                if (t == Poweruptype::Star)
                {
                    starChoices[0] = Poweruptype::speedboost;
                    starChoices[1] = Poweruptype::snowballpower;
                    starChoices[2] = Poweruptype::ballonmode;
                    starChoiceSelected = 0;
                    starChoiceActive = true;

                    starChoiceBoxes[0].setFillColor(sf::Color(255, 200, 0, 220));
                    starChoiceBoxes[1].setFillColor(sf::Color(100, 200, 255, 220));
                    starChoiceBoxes[2].setFillColor(sf::Color(255, 100, 200, 220));
                    starChoiceTexts[0].setString("Speed Boost");
                    starChoiceTexts[1].setString("Snowball Power");
                    starChoiceTexts[2].setString("Balloon Mode");
                }
                else
                {
                    players[pi].applypowerup(t);
                }
            }
        }
    }
}

void Game::fireEnemyProjectiles()
{
    float tx = players[0].isalive() ? players[0].getX() : 400;
    float ty = players[0].isalive() ? players[0].getY() : 300;

    Projectile* freeProj = nullptr;

    for (int i = 0; i < activeTornados; i++)
    {
        if (!tornados[i].isalive() || tornados[i].isencased()) continue;
        if (tornados[i].firedknife())
        {
            tornados[i].resetknifeflag();
            freeProj = nullptr;
            for (int j = 0; j < MAX_PROJECTILES; j++)
            {
                if (!projectiles[j].isalive())
                {
                    freeProj = &projectiles[j];
                    break;
                }
            }
            if (freeProj)
            {
                float kx, ky;
                tornados[i].getorigin_knife(kx, ky);
                freeProj->launch(kx, ky, tx, ty, ProjectileType::Knife);
            }
        }
    }

    if (isGamakichiLevel && gamakichi.isalive() && gamakichi.wantstofire())
    {
        gamakichi.resetfireflag();
        float origins[4][2];
        int count = 0;
        gamakichi.getfireorigins(origins, count);
        for (int i = 0; i < count; i++)
        {
            freeProj = nullptr;
            for (int j = 0; j < MAX_PROJECTILES; j++)
            {
                if (!projectiles[j].isalive())
                {
                    freeProj = &projectiles[j];
                    break;
                }
            }
            if (freeProj)
                freeProj->launch(origins[i][0], origins[i][1], tx, ty, ProjectileType::Artillery);
        }
    }
}
void Game::trySpawnMogeraChild()
{
    if (!mogera.isalive()) return;
    if (mogera.wantstospawnchild())
    {
        mogera.resetspawnflag();
        if (activeMogeraChildren < 8)
        {
            float cx, cy;
            mogera.getspawnposition(cx, cy);
            mogeraChildren[activeMogeraChildren].init(cx, cy);
            activeMogeraChildren++;
        }
    }
}

void Game::tryFireGamakichi()
{
}
void Game::checkLevelComplete()
{
    if (screen != GameScreen::Playing) return;

    bool anyAlive = false;
    for (int pi = 0; pi < activePlayerCount; pi++)
        if (players[pi].isalive()) anyAlive = true;
    if (!anyAlive) return;

    if (!allEnemiesDead()) return;
    onLevelComplete();
}

bool Game::allEnemiesDead()
{
    for (int i = 0; i < activeBottoms; i++)
        if (botoms[i].isalive() && !botoms[i].isencased()) return false;
    for (int i = 0; i < activeFoogas; i++)
        if (foogas[i].isalive() && !foogas[i].isencased()) return false;
    for (int i = 0; i < activeTornados; i++)
        if (tornados[i].isalive() && !tornados[i].isencased()) return false;
    for (int i = 0; i < activeMogeraChildren; i++)
        if (mogeraChildren[i].isalive() && !mogeraChildren[i].isencased()) return false;
    if (isMogeraLevel && mogera.isalive()) return false;
    if (isGamakichiLevel && gamakichi.isalive()) return false;
    // Force-kill any remaining encased enemies so they cannot escape back to Walking
    for (int i = 0; i < activeBottoms; i++)
        if (botoms[i].isalive()) botoms[i].startrolling();
    for (int i = 0; i < activeFoogas; i++)
        if (foogas[i].isalive()) foogas[i].startrolling();
    for (int i = 0; i < activeTornados; i++)
        if (tornados[i].isalive()) tornados[i].startrolling();
    return true;
}

void Game::onLevelComplete()
{
    levelCompleteTimer = 0;
    levelCompleteTitle.setString("Level " + inttostr(currentLevelNumber) + " Complete!");
    int totalScore = players[0].getstate().score;
    if (activePlayerCount > 1)
        totalScore += players[1].getstate().score;
    levelCompleteScore.setString("Score: " + inttostr(totalScore));
    screen = GameScreen::LevelComplete;
}

void Game::onGameOver()
{
    saveCurrentProgress();
    gameOverScore.setString("Final Score: " + inttostr(players[0].getstate().score));
    FileManager::getinstance().saveLeaderboardEntry(FileManager::getinstance().getLoggedInUser(),
        players[0].getstate().score,
        currentLevelNumber
    );
    screen = GameScreen::GameOver;
}

void Game::applyChainScore(int chain, int pi)
{
    int base = 300;
    float mult = 1.0f + chain * 0.1f;
    int score = (int)(base * mult);
    players[pi].addscore(score);
}

void Game::spawnPowerUp(float px, float py, Poweruptype t)
{
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (!powerups[i].isalive())
        {
            powerups[i].spawn(px, py, t);
            return;
        }
    }
}

void Game::pickBonusLevels()
{
    memset(levelIsBonusMap, 0, sizeof(levelIsBonusMap));
}

void Game::saveCurrentProgress()
{
    std::string user = FileManager::getinstance().getLoggedInUser();
    if (user.empty()) return;
    FileManager::getinstance().saveProgress(
        user,
        currentLevelNumber,
        players[0].getstate().lives,
        players[0].getstate().gems,
        players[0].getstate().score
    );
}

void Game::buyShopItem(int idx)
{
    int costs[5] = {
        SHOP_LIFE_COST,
        SHOP_SPEED_COST,
        SHOP_SNOWBALL_COST,
        SHOP_DISTANCE_COST,
        SHOP_BALLOON_COST
    };
    Poweruptype types[5] = {
        Poweruptype::extralife,
        Poweruptype::speedboost,
        Poweruptype::snowballpower,
        Poweruptype::increaseindistance,
        Poweruptype::ballonmode
    };

    int pi = 0;
    if (players[pi].getstate().gems >= costs[idx])
    {
        players[pi].getstate().gems -= costs[idx];
        players[pi].applypowerup(types[idx]);
        shopGemsText.setString("Your Gems: " + inttostr(players[pi].getstate().gems));
    }
}

void Game::updateHUDText()
{
    for (int pi = 0; pi < activePlayerCount; pi++)
    {
        scoreText[pi].setString("P" + inttostr(pi + 1) + " Score: " + inttostr(players[pi].getstate().score));
        livesText[pi].setString("Lives: " + inttostr(players[pi].getstate().lives));

        std::string pstr = "";
        if (players[pi].getstate().speedBoostActive)
            pstr += "SPD ";
        if (players[pi].getstate().snowballPowerActive)
            pstr += "PWR ";
        if (players[pi].getstate().balloonActive)
            pstr += "BAL ";
        powerupText[pi].setString(pstr);
    }
    gemsText.setString("Gems: " + inttostr(players[0].getstate().gems));
}

void Game::appendChar(char* buf, int maxLen, char c)
{
    int len = strlen(buf);
    if (len < maxLen)
    {
        buf[len] = c;
        buf[len + 1] = '\0';
    }
}

void Game::deleteChar(char* buf)
{
    int len = strlen(buf);
    if (len > 0) buf[len - 1] = '\0';
}

std::string Game::maskString(const char* buf)
{
    std::string s(buf);
    std::string masked(s.size(), '*');
    return masked;
}

void Game::initUI()
{
    initSplash();
    initLoginUI();
    initCharSelectUI();
    initPauseUI();
    initShopUI();
    initGameOverUI();
    initLevelCompleteUI();
    initBossDefeatedUI();
    initHUD();
    leaderboardScreen.init(font);
}

void Game::initSplash()
{
    splashBg.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    splashBg.setFillColor(sf::Color(8, 12, 40));
    splashTitle.setFont(font);
    splashTitle.setString("SNOW BROS");
    splashTitle.setCharacterSize(72);
    splashTitle.setFillColor(sf::Color(100, 200, 255));
    splashTitle.setStyle(sf::Text::Bold);
    splashTitle.setPosition(180, 200);
    splashSubtitle.setFont(font);
    splashSubtitle.setString("");
    splashSubtitle.setCharacterSize(26);
    splashSubtitle.setFillColor(sf::Color(200, 200, 255));
    splashSubtitle.setPosition(230, 300);
}
void Game::initLoginUI()
{
    titleText.setFont(font);
    titleText.setString("Login");
    titleText.setCharacterSize(42);
    titleText.setFillColor(sf::Color(100, 200, 255));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(320, 100);
    float startY = 210;
    std::string labels[2] = { "Username", "Password" };
    for (int i = 0; i < 2; i++)
    {
        inputBoxes[i].setSize(sf::Vector2f(400, 44));
        inputBoxes[i].setPosition(200, startY + i * 90);
        inputBoxes[i].setFillColor(sf::Color(30, 30, 60));
        inputBoxes[i].setOutlineColor(sf::Color(80, 120, 200));
        inputBoxes[i].setOutlineThickness(2);
        inputLabels[i].setFont(font);
        inputLabels[i].setString(labels[i]);
        inputLabels[i].setCharacterSize(18);
        inputLabels[i].setFillColor(sf::Color(180, 180, 220));
        inputLabels[i].setPosition(200, startY + i * 90 - 26);
        inputTexts[i].setFont(font);
        inputTexts[i].setCharacterSize(20);
        inputTexts[i].setFillColor(sf::Color::White);
        inputTexts[i].setPosition(208, startY + i * 90 + 10);
    }
    loginBtn.setSize(sf::Vector2f(180, 44));
    loginBtn.setPosition(200, 420);
    loginBtn.setFillColor(sf::Color(60, 120, 200));
    loginBtnText.setFont(font);
    loginBtnText.setString("Login");
    loginBtnText.setCharacterSize(22);
    loginBtnText.setFillColor(sf::Color::White);
    loginBtnText.setPosition(250, 428);
    switchBtn.setSize(sf::Vector2f(220, 44));
    switchBtn.setPosition(420, 420);
    switchBtn.setFillColor(sf::Color(40, 80, 140));
    switchBtnText.setFont(font);
    switchBtnText.setString("Create Account");
    switchBtnText.setCharacterSize(20);
    switchBtnText.setFillColor(sf::Color(180, 200, 255));
    switchBtnText.setPosition(428, 428);
    leaderboardBtn.setSize(sf::Vector2f(220, 44));
    leaderboardBtn.setPosition(200, 480);
    leaderboardBtn.setFillColor(sf::Color(60, 40, 120));
    leaderboardBtnText.setFont(font);
    leaderboardBtnText.setString("Leaderboard");
    leaderboardBtnText.setCharacterSize(20);
    leaderboardBtnText.setFillColor(sf::Color(200, 180, 255));
    leaderboardBtnText.setPosition(218, 488);
}
void Game::initSignupUI()
{
    titleText.setString("Create Account");
    titleText.setPosition(200, 100);
    signupBtn.setSize(sf::Vector2f(180, 44));
    signupBtn.setPosition(200, 420);
    signupBtn.setFillColor(sf::Color(60, 180, 80));
    signupBtnText.setFont(font);
    signupBtnText.setString("Sign Up");
    signupBtnText.setCharacterSize(22);
    signupBtnText.setFillColor(sf::Color::White);
    signupBtnText.setPosition(220, 428);
    switchBtnText.setString("Back to Login");
    float startY = 210;
    std::string labels[2] = { "Username", "Password" };
    for (int i = 0; i < 2; i++)
    {
        inputBoxes[i].setPosition(200, startY + i * 90);
        inputLabels[i].setString(labels[i]);
        inputLabels[i].setPosition(200, startY + i * 90 - 26);
        inputTexts[i].setString("");
        inputTexts[i].setPosition(208, startY + i * 90 + 10);
    }
}
void Game::initCharSelectUI()
{
    charSelectTitle.setFont(font);
    charSelectTitle.setCharacterSize(36);
    charSelectTitle.setFillColor(sf::Color(100, 200, 255));
    charSelectTitle.setStyle(sf::Text::Bold);
    charSelectPrompt.setFont(font);
    charSelectPrompt.setCharacterSize(20);
    charSelectPrompt.setFillColor(sf::Color(200, 200, 220));
    if (charSelectStep == 0)
    {
        charSelectTitle.setString("Select Game Mode");
        charSelectTitle.setPosition(220, 120);
        charSelectPrompt.setString("How many players?");
        charSelectPrompt.setPosition(290, 180);
        modeBtn[0].setSize(sf::Vector2f(240, 60));
        modeBtn[0].setPosition(130, 260);
        modeBtn[0].setFillColor(sf::Color(60, 100, 180));
        modeBtnText[0].setFont(font);
        modeBtnText[0].setString("Single Player");
        modeBtnText[0].setCharacterSize(24);
        modeBtnText[0].setFillColor(sf::Color::White);
        modeBtnText[0].setPosition(150, 274);
        modeBtn[1].setSize(sf::Vector2f(240, 60));
        modeBtn[1].setPosition(430, 260);
        modeBtn[1].setFillColor(sf::Color(100, 60, 180));
        modeBtnText[1].setFont(font);
        modeBtnText[1].setString("2 Players");
        modeBtnText[1].setCharacterSize(24);
        modeBtnText[1].setFillColor(sf::Color::White);
        modeBtnText[1].setPosition(466, 274);
    }
    else
    {
        int pi = multiplayerMode ? (charSelectStep - 1) : 0;
        charSelectTitle.setString("Player " + inttostr(pi + 1) + " - Choose Character");
        charSelectTitle.setPosition(160, 80);
        charSelectPrompt.setString("Click a character then press Confirm");
        charSelectPrompt.setPosition(200, 130);

        std::string cnames[5] = { "Nick","Blue","Green","Red","Yellow" };
        sf::Color   ccols[5] = {
            sf::Color(60,100,180),
            sf::Color(40,80,160),
            sf::Color(40,120,60),
            sf::Color(160,40,40),
            sf::Color(160,140,20)
        };

        for (int i = 0; i < 5; i++)
        {
            charCards[i].setSize(sf::Vector2f(130, 150));
            charCards[i].setPosition(30 + i * 150, 180);
            charCards[i].setFillColor(i == charSelectIndex[pi] ? sf::Color(80, 140, 220) : sf::Color(40, 40, 80));
            charCards[i].setOutlineColor(sf::Color(100, 140, 200));
            charCards[i].setOutlineThickness(2);

            charNames[i].setFont(font);
            charNames[i].setString(cnames[i]);
            charNames[i].setCharacterSize(18);
            charNames[i].setFillColor(sf::Color::White);
            charNames[i].setPosition(40 + i * 150, 310);
        }

        charSelectConfirm.setSize(sf::Vector2f(200, 50));
        charSelectConfirm.setPosition(300, 400);
        charSelectConfirm.setFillColor(sf::Color(60, 180, 80));

        charSelectConfirmText.setFont(font);
        charSelectConfirmText.setString("Confirm");
        charSelectConfirmText.setCharacterSize(24);
        charSelectConfirmText.setFillColor(sf::Color::White);
        charSelectConfirmText.setPosition(350, 410);
    }
}

void Game::initPauseUI()
{
    pauseBg.setSize(sf::Vector2f(320, 280));
    pauseBg.setPosition(240, 160);
    pauseBg.setFillColor(sf::Color(20, 20, 50, 230));
    pauseBg.setOutlineColor(sf::Color(80, 120, 200));
    pauseBg.setOutlineThickness(2);

    pauseTitle.setFont(font);
    pauseTitle.setString("PAUSED");
    pauseTitle.setCharacterSize(34);
    pauseTitle.setFillColor(sf::Color(100, 200, 255));
    pauseTitle.setPosition(330, 175);

    std::string pbLabels[4] = { "Resume","Shop","Save Game","Logout" };
    for (int i = 0; i < 4; i++)
    {
        pauseButtons[i].setSize(sf::Vector2f(240, 40));
        pauseButtons[i].setPosition(280, 230 + i * 52);
        pauseButtons[i].setFillColor(sf::Color(40, 60, 120));
        pauseButtons[i].setOutlineColor(sf::Color(80, 120, 200));
        pauseButtons[i].setOutlineThickness(1);

        pauseButtonText[i].setFont(font);
        pauseButtonText[i].setString(pbLabels[i]);
        pauseButtonText[i].setCharacterSize(20);
        pauseButtonText[i].setFillColor(sf::Color::White);
        pauseButtonText[i].setPosition(340, 240 + i * 52);
    }
}

void Game::initShopUI()
{
    shopBg.setSize(sf::Vector2f(440, 380));
    shopBg.setPosition(180, 110);
    shopBg.setFillColor(sf::Color(15, 15, 40, 240));
    shopBg.setOutlineColor(sf::Color(100, 160, 255));
    shopBg.setOutlineThickness(2);

    shopTitle.setFont(font);
    shopTitle.setString("SHOP");
    shopTitle.setCharacterSize(34);
    shopTitle.setFillColor(sf::Color(255, 220, 60));
    shopTitle.setPosition(360, 120);

    shopGemsText.setFont(font);
    shopGemsText.setCharacterSize(18);
    shopGemsText.setFillColor(sf::Color(80, 220, 180));
    shopGemsText.setPosition(190, 158);

    std::string itemLabels[5] = {
        "Extra Life       50 gems",
        "Speed Boost      20 gems",
        "Snowball Power   30 gems",
        "Distance Up      25 gems",
        "Balloon Mode     35 gems"
    };

    for (int i = 0; i < 5; i++)
    {
        shopItems[i].setSize(sf::Vector2f(380, 40));
        shopItems[i].setPosition(210, 186 + i * 50);
        shopItems[i].setFillColor(sf::Color(30, 50, 100));
        shopItems[i].setOutlineColor(sf::Color(60, 100, 180));
        shopItems[i].setOutlineThickness(1);

        shopItemText[i].setFont(font);
        shopItemText[i].setString(itemLabels[i]);
        shopItemText[i].setCharacterSize(18);
        shopItemText[i].setFillColor(sf::Color::White);
        shopItemText[i].setPosition(220, 196 + i * 50);
    }

    shopCloseBtn.setSize(sf::Vector2f(140, 40));
    shopCloseBtn.setPosition(330, 440);
    shopCloseBtn.setFillColor(sf::Color(140, 40, 40));

    shopCloseBtnText.setFont(font);
    shopCloseBtnText.setString("Close");
    shopCloseBtnText.setCharacterSize(20);
    shopCloseBtnText.setFillColor(sf::Color::White);
    shopCloseBtnText.setPosition(370, 448);
}

void Game::initGameOverUI()
{
    gameOverTitle.setFont(font);
    gameOverTitle.setString("GAME OVER");
    gameOverTitle.setCharacterSize(56);
    gameOverTitle.setFillColor(sf::Color(220, 60, 60));
    gameOverTitle.setStyle(sf::Text::Bold);
    gameOverTitle.setPosition(200, 140);

    gameOverScore.setFont(font);
    gameOverScore.setCharacterSize(28);
    gameOverScore.setFillColor(sf::Color(220, 220, 100));
    gameOverScore.setPosition(240, 240);

    gameOverPrompt.setFont(font);
    gameOverPrompt.setString("");
    gameOverPrompt.setCharacterSize(20);
    gameOverPrompt.setFillColor(sf::Color(180, 180, 220));
    gameOverPrompt.setPosition(240, 300);

    gameOverRetry.setSize(sf::Vector2f(200, 50));
    gameOverRetry.setPosition(150, 380);
    gameOverRetry.setFillColor(sf::Color(60, 120, 200));

    gameOverRetryText.setFont(font);
    gameOverRetryText.setString("Play Again");
    gameOverRetryText.setCharacterSize(22);
    gameOverRetryText.setFillColor(sf::Color::White);
    gameOverRetryText.setPosition(175, 390);

    gameOverMenu.setSize(sf::Vector2f(200, 50));
    gameOverMenu.setPosition(450, 380);
    gameOverMenu.setFillColor(sf::Color(80, 40, 120));

    gameOverMenuText.setFont(font);
    gameOverMenuText.setString("Main Menu");
    gameOverMenuText.setCharacterSize(22);
    gameOverMenuText.setFillColor(sf::Color::White);
    gameOverMenuText.setPosition(473, 390);
}

void Game::initLevelCompleteUI()
{
    levelCompleteTitle.setFont(font);
    levelCompleteTitle.setCharacterSize(46);
    levelCompleteTitle.setFillColor(sf::Color(100, 255, 140));
    levelCompleteTitle.setStyle(sf::Text::Bold);
    levelCompleteTitle.setPosition(160, 180);

    levelCompleteScore.setFont(font);
    levelCompleteScore.setCharacterSize(26);
    levelCompleteScore.setFillColor(sf::Color(220, 220, 100));
    levelCompleteScore.setPosition(240, 270);

    levelCompletePrompt.setFont(font);
    levelCompletePrompt.setString("Get ready for next level...");
    levelCompletePrompt.setCharacterSize(20);
    levelCompletePrompt.setFillColor(sf::Color(180, 180, 220));
    levelCompletePrompt.setPosition(240, 330);
}

void Game::initBossDefeatedUI()
{
    bossDefeatedTitle.setFont(font);
    bossDefeatedTitle.setCharacterSize(48);
    bossDefeatedTitle.setFillColor(sf::Color(255, 220, 60));
    bossDefeatedTitle.setStyle(sf::Text::Bold);
    bossDefeatedTitle.setPosition(140, 180);

    bossDefeatedReward.setFont(font);
    bossDefeatedReward.setCharacterSize(26);
    bossDefeatedReward.setFillColor(sf::Color(80, 220, 180));
    bossDefeatedReward.setPosition(200, 270);
}

void Game::initHUD()
{
    hudBg.setSize(sf::Vector2f(WINDOW_WIDTH, 26));
    hudBg.setPosition(0, 0);
    hudBg.setFillColor(sf::Color(0, 0, 0, 180));

    for (int pi = 0; pi < MAX_PLAYERS; pi++)
    {
        scoreText[pi].setFont(font);
        scoreText[pi].setCharacterSize(15);
        scoreText[pi].setFillColor(sf::Color::White);
        scoreText[pi].setPosition(pi == 0 ? 4 : 300, 4);

        livesText[pi].setFont(font);
        livesText[pi].setCharacterSize(15);
        livesText[pi].setFillColor(sf::Color(255, 160, 160));
        livesText[pi].setPosition(pi == 0 ? 180 : 480, 4);

        powerupText[pi].setFont(font);
        powerupText[pi].setCharacterSize(13);
        powerupText[pi].setFillColor(sf::Color(255, 220, 60));
        powerupText[pi].setPosition(pi == 0 ? 4 : 300, 590);
    }

    gemsText.setFont(font);
    gemsText.setCharacterSize(15);
    gemsText.setFillColor(sf::Color(80, 220, 180));
    gemsText.setPosition(660, 4);

    levelText.setFont(font);
    levelText.setCharacterSize(15);
    levelText.setFillColor(sf::Color(200, 200, 255));
    levelText.setPosition(350, 4);

    bossBarBg.setSize(sf::Vector2f(240, 14));
    bossBarBg.setPosition(280, 30);
    bossBarBg.setFillColor(sf::Color(50, 50, 50));

    bossBarFill.setSize(sf::Vector2f(240, 14));
    bossBarFill.setPosition(280, 30);
    bossBarFill.setFillColor(sf::Color(80, 220, 80));

    bossBarLabel.setFont(font);
    bossBarLabel.setString("BOSS");
    bossBarLabel.setCharacterSize(12);
    bossBarLabel.setFillColor(sf::Color::White);
    bossBarLabel.setPosition(530, 31);

    for (int i = 0; i < 3; i++)
    {
        starChoiceBoxes[i].setSize(sf::Vector2f(180, 80));
        starChoiceBoxes[i].setPosition(80 + i * 220, 260);
        starChoiceBoxes[i].setOutlineColor(sf::Color::White);
        starChoiceBoxes[i].setOutlineThickness(2);

        starChoiceTexts[i].setFont(font);
        starChoiceTexts[i].setCharacterSize(18);
        starChoiceTexts[i].setFillColor(sf::Color::White);
        starChoiceTexts[i].setPosition(90 + i * 220, 295);
    }

    starChoiceTitle.setFont(font);
    starChoiceTitle.setString("STAR BONUS - Pick a Power-Up!");
    starChoiceTitle.setCharacterSize(26);
    starChoiceTitle.setFillColor(sf::Color(255, 255, 60));
    starChoiceTitle.setPosition(160, 210);
}
int Game::getTotalEnemyCount()
{
    int count = 0;
    for (int i = 0; i < activeBottoms; i++)
        if (botoms[i].isalive()) count++;
    for (int i = 0; i < activeFoogas; i++)
        if (foogas[i].isalive()) count++;
    for (int i = 0; i < activeTornados; i++)
        if (tornados[i].isalive()) count++;
    for (int i = 0; i < activeMogeraChildren; i++)
        if (mogeraChildren[i].isalive()) count++;
    if (isMogeraLevel && mogera.isalive()) count++;
    if (isGamakichiLevel && gamakichi.isalive()) count++;
    return count;
}
void Game::handleLeaderboardEvent(sf::Event& e)
{
    bool goBack = false;
    leaderboardScreen.handleEvent(e, goBack);
    if (goBack)
        screen = GameScreen::Login;
}