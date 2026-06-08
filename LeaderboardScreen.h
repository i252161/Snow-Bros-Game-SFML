#pragma once
#include <SFML/Graphics.hpp>
#include "FileManager.h"

class Leaderboardscreen
{
public:
    Leaderboardscreen();

    void init(sf::Font& font);
    void refresh();
    void render(sf::RenderWindow& window);
    void handleEvent(sf::Event& e, bool& goBack);

private:
    sf::Font* font;

    LeaderboardEntry entries[MAX_LEADERBOARD_ENTRIES];
    int entryCount;

    sf::RectangleShape bg;
    sf::Text title;
    sf::RectangleShape rowBg[MAX_LEADERBOARD_ENTRIES];
    sf::Text entryText[MAX_LEADERBOARD_ENTRIES];
    sf::RectangleShape closeBtn;
    sf::Text closeBtnText;

    static std::string inttostr(int v);
};