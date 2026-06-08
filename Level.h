#pragma once
#include "Platform.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

enum class LevelType
{
    Standard,
    Boss,
    Bonus
};

struct EnemySpawnInfo
{
    float x;
    float y;
    int enemyKind;
    int variant;
};

struct LevelConfig
{
    int levelNumber;
    LevelType type;
    int enemyCount;
    EnemySpawnInfo enemies[MAX_ENEMIES];
    int platformcount;
    sf::Color bgColor;
    sf::Color platformColor;
    int bonusGemReward;
};

class Level
{
public:
    Level();

    void load(int levelNumber, bool isbonus);
    void draw(sf::RenderWindow& window);

    Platform* getplatforms();
    int getplatformcount() const;

    LevelConfig& getconfig();
    int getlevelnumber() const;
    bool isbonus() const;
    bool isbosslevel() const;

private:
    Platform platforms[MAX_PLATFORMS];
    int platformcount;
    LevelConfig config;
    bool bonusLevel;

    sf::RectangleShape background;

    void buildLevel1();
    void buildLevel2();
    void buildLevel3();
    void buildLevel4();
    void buildLevel5();
    void buildLevel6();
    void buildLevel7();
    void buildLevel8();
    void buildLevel9();
    void buildLevel10();

    void addPlatform(float x, float y, float w, float h, sf::Color col);
    void addEnemy(int kind, float x, float y, int variant);
    void clearenemies();
};