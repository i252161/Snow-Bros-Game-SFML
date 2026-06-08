#include "Level.h"
#include "Constants.h"

Level::Level()
{
    platformcount = 0;
    bonusLevel = false;

    background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void Level::load(int levelnumber, bool isbonus)
{
    platformcount = 0;
    config.levelNumber = levelnumber;
    config.enemyCount = 0;
    config.bonusGemReward = 0;
    bonusLevel = isbonus;

    switch (levelnumber)
    {
    case 1:  buildLevel1();
        break;
    case 2:  buildLevel2();
        break;
    case 3:  buildLevel3();
        break;
    case 4:  buildLevel4();
        break;
    case 5:  buildLevel5();
        break;
    case 6:  buildLevel6();
        break;
    case 7:  buildLevel7();
        break;
    case 8:  buildLevel8();
        break;
    case 9:  buildLevel9();
        break;
    case 10: buildLevel10();
        break;
    default: buildLevel1();
        break;
    }
    background.setFillColor(config.bgColor);

    if (isbonus)
    {
        config.type = LevelType::Bonus;
        config.bonusGemReward = 50;
    }
}

void Level::addPlatform(float x, float y, float w, float h, sf::Color col)
{
    if (platformcount >= MAX_PLATFORMS)
        return;
    platforms[platformcount] = Platform(x, y, w, h, col);
    platformcount++;
}

void Level::addEnemy(int kind, float x, float y, int variant)
{
    if (config.enemyCount >= MAX_ENEMIES)
        return;
    config.enemies[config.enemyCount].x = x;
    config.enemies[config.enemyCount].y = y;
    config.enemies[config.enemyCount].enemyKind = kind;
    config.enemies[config.enemyCount].variant = variant;
    config.enemyCount++;
}

void Level::clearenemies()
{
    config.enemyCount = 0;
}

void Level::buildLevel1()
{
    config.bgColor = sf::Color(30, 60, 120);
    config.platformColor = sf::Color(100, 70, 40);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(50, 460, 220, 16, pc);
    addPlatform(530, 460, 220, 16, pc);
    addPlatform(200, 360, 400, 16, pc);
    addPlatform(0, 270, 180, 16, pc);
    addPlatform(620, 270, 180, 16, pc);
    addPlatform(250, 180, 300, 16, pc);

    addEnemy(0, 100, 430, 0);
    addEnemy(0, 600, 420, 0);
    addEnemy(0, 300, 330, 0);
}

void Level::buildLevel2()
{
    config.bgColor = sf::Color(20, 80, 60);
    config.platformColor = sf::Color(80, 120, 60);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(100, 470, 200, 16, pc);
    addPlatform(500, 470, 200, 16, pc);
    addPlatform(280, 370, 240, 16, pc);
    addPlatform(0, 280, 160, 16, pc);
    addPlatform(640, 280, 160, 16, pc);
    addPlatform(320, 190, 160, 16, pc);

    addEnemy(0, 150, 540, 0);
    addEnemy(0, 550, 540, 0);
    addEnemy(1, 300, 340, 0);
    addEnemy(1, 100, 250, 0);
}

void Level::buildLevel3()
{
    config.bgColor = sf::Color(60, 20, 80);
    config.platformColor = sf::Color(100, 60, 140);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(60, 480, 180, 16, pc);
    addPlatform(560, 480, 180, 16, pc);
    addPlatform(240, 390, 320, 16, pc);
    addPlatform(0, 300, 140, 16, pc);
    addPlatform(660, 300, 140, 16, pc);
    addPlatform(280, 210, 240, 16, pc);
    addPlatform(100, 130, 160, 16, pc);
    addPlatform(540, 130, 160, 16, pc);

    addEnemy(0, 100, 550, 0);
    addEnemy(0, 600, 550, 0);
    addEnemy(1, 280, 360, 0);
    addEnemy(2, 300, 180, 0);
}

void Level::buildLevel4()
{
    config.bgColor = sf::Color(80, 40, 20);
    config.platformColor = sf::Color(140, 90, 50);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(80, 490, 160, 16, pc);
    addPlatform(560, 490, 160, 16, pc);
    addPlatform(300, 410, 200, 16, pc);
    addPlatform(0, 330, 120, 16, pc);
    addPlatform(680, 330, 120, 16, pc);
    addPlatform(200, 250, 160, 16, pc);
    addPlatform(440, 250, 160, 16, pc);
    addPlatform(320, 160, 160, 16, pc);

    addEnemy(0, 120, 550, 0);
    addEnemy(0, 620, 550, 0);
    addEnemy(1, 320, 380, 0);
    addEnemy(1, 200, 220, 0);
    addEnemy(2, 400, 220, 0);
    addEnemy(2, 320, 130, 0);
}

void Level::buildLevel5()
{
    config.bgColor = sf::Color(10, 10, 30);
    config.platformColor = sf::Color(60, 30, 80);
    config.type = LevelType::Boss;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(0, 400, 200, 16, pc);
    addPlatform(600, 400, 200, 16, pc);
    addPlatform(250, 300, 300, 16, pc);

    addEnemy(3, 300, 380, 0);
}

void Level::buildLevel6()
{
    config.bgColor = sf::Color(100, 30, 10);
    config.platformColor = sf::Color(160, 80, 20);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(80, 480, 180, 16, pc);
    addPlatform(540, 480, 180, 16, pc);
    addPlatform(260, 380, 280, 16, pc);
    addPlatform(0, 290, 160, 16, pc);
    addPlatform(640, 290, 160, 16, pc);
    addPlatform(300, 200, 200, 16, pc);

    addEnemy(0, 120, 550, 1);
    addEnemy(0, 600, 550, 1);
    addEnemy(1, 280, 350, 1);
    addEnemy(1, 100, 260, 1);
    addEnemy(2, 400, 170, 1);
}

void Level::buildLevel7()
{
    config.bgColor = sf::Color(20, 20, 60);
    config.platformColor = sf::Color(40, 40, 100);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(60, 490, 160, 16, pc);
    addPlatform(580, 490, 160, 16, pc);
    addPlatform(260, 400, 280, 16, pc);
    addPlatform(0, 310, 140, 16, pc);
    addPlatform(660, 310, 140, 16, pc);
    addPlatform(200, 220, 180, 16, pc);
    addPlatform(420, 220, 180, 16, pc);
    addPlatform(310, 130, 180, 16, pc);

    addEnemy(0, 100, 550, 1);
    addEnemy(0, 650, 550, 1);
    addEnemy(1, 280, 370, 2);
    addEnemy(1, 100, 280, 1);
    addEnemy(2, 240, 190, 1);
    addEnemy(2, 440, 190, 1);
}

void Level::buildLevel8()
{
    config.bgColor = sf::Color(10, 40, 10);
    config.platformColor = sf::Color(30, 100, 30);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(100, 490, 160, 16, pc);
    addPlatform(540, 490, 160, 16, pc);
    addPlatform(280, 400, 240, 16, pc);
    addPlatform(0, 310, 130, 16, pc);
    addPlatform(670, 310, 130, 16, pc);
    addPlatform(220, 220, 360, 16, pc);
    addPlatform(300, 130, 200, 16, pc);

    addEnemy(0, 150, 550, 2);
    addEnemy(0, 600, 550, 2);
    addEnemy(1, 300, 370, 2);
    addEnemy(2, 100, 280, 2);
    addEnemy(2, 680, 280, 2);
    addEnemy(0, 300, 190, 2);
    addEnemy(1, 350, 100, 2);
}

void Level::buildLevel9()
{
    config.bgColor = sf::Color(60, 60, 10);
    config.platformColor = sf::Color(120, 120, 20);
    config.type = LevelType::Standard;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(80, 490, 140, 16, pc);
    addPlatform(580, 490, 140, 16, pc);
    addPlatform(270, 400, 260, 16, pc);
    addPlatform(0, 310, 120, 16, pc);
    addPlatform(680, 310, 120, 16, pc);
    addPlatform(180, 220, 200, 16, pc);
    addPlatform(420, 220, 200, 16, pc);
    addPlatform(290, 130, 220, 16, pc);

    addEnemy(0, 120, 550, 2);
    addEnemy(0, 640, 550, 2);
    addEnemy(1, 290, 370, 2);
    addEnemy(2, 100, 280, 2);
    addEnemy(2, 690, 280, 2);
    addEnemy(0, 210, 190, 2);
    addEnemy(1, 430, 190, 2);
    addEnemy(2, 330, 100, 2);
}

void Level::buildLevel10()
{
    config.bgColor = sf::Color(5, 5, 20);
    config.platformColor = sf::Color(80, 20, 40);
    config.type = LevelType::Boss;

    sf::Color pc = config.platformColor;
    addPlatform(0, 580, 800, 20, pc);
    addPlatform(0, 420, 160, 16, pc);
    addPlatform(640, 420, 160, 16, pc);
    addPlatform(200, 320, 400, 16, pc);

    addEnemy(4, 270, 360, 0);
}

void Level::draw(sf::RenderWindow& window)
{
    window.draw(background);

    for (int i = 0; i < platformcount; i++)
        platforms[i].draw(window);
}

Platform* Level::getplatforms()
{
    return platforms;
}
int Level::getplatformcount() const
{
    return platformcount;
}
LevelConfig& Level::getconfig()
{
    return config;
}
int Level::getlevelnumber() const
{
    return config.levelNumber;
}
bool Level::isbonus() const
{
    return bonusLevel;
}
bool Level::isbosslevel() const
{
    return config.type == LevelType::Boss;
}