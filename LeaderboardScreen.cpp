#include "LeaderboardScreen.h"
#include <sstream>
using namespace std;

string Leaderboardscreen::inttostr(int v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

Leaderboardscreen::Leaderboardscreen()
{
    font = nullptr;
    entryCount = 0;
}

void Leaderboardscreen::init(sf::Font& f)
{
    font = &f;
    bg.setSize(sf::Vector2f(700, 520));
    bg.setPosition(50, 40);
    bg.setFillColor(sf::Color(10, 10, 40, 240));
    bg.setOutlineColor(sf::Color(100, 100, 220));
    bg.setOutlineThickness(2);
    title.setFont(*font);
    title.setString("LEADERBOARD - TOP 10");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color(255, 220, 60));
    title.setPosition(200, 55);
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++)
    {
        rowBg[i].setSize(sf::Vector2f(660, 36));
        rowBg[i].setPosition(70, 110 + i * 38);

        if (i % 2 == 0)
            rowBg[i].setFillColor(sf::Color(30, 30, 70, 200));
        else
            rowBg[i].setFillColor(sf::Color(20, 20, 55, 200));
        entryText[i].setFont(*font);
        entryText[i].setCharacterSize(18);
        entryText[i].setFillColor(sf::Color::White);
        entryText[i].setPosition(80, 117 + i * 38);
    }
    closeBtn.setSize(sf::Vector2f(180, 40));
    closeBtn.setPosition(310, 510);
    closeBtn.setFillColor(sf::Color(80, 40, 120));
    closeBtn.setOutlineColor(sf::Color(160, 100, 220));
    closeBtn.setOutlineThickness(2);
    closeBtnText.setFont(*font);
    closeBtnText.setString("BACK");
    closeBtnText.setCharacterSize(20);
    closeBtnText.setFillColor(sf::Color::White);
    closeBtnText.setPosition(375, 518);
}
void Leaderboardscreen::refresh()
{
    entryCount = FileManager::getinstance().loadleaderboard(entries, MAX_LEADERBOARD_ENTRIES);

    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++)
    {
        if (i < entryCount)
        {
            std::string rank;
            if (i == 0)
                rank = "1st";
            else if (i == 1)
                rank = "2nd";
            else if (i == 2)
                rank = "3rd";
            else
                rank = inttostr(i + 1) + "th";

            std::string line = rank + "  " + std::string(entries[i].username) +
                "    Score: " + inttostr(entries[i].score) + "    Level: " + inttostr(entries[i].levelReached);

            entryText[i].setString(line);

            if (i == 0)
                entryText[i].setFillColor(sf::Color(255, 215, 0));
            else if (i == 1)
                entryText[i].setFillColor(sf::Color(200, 200, 200));
            else if (i == 2)
                entryText[i].setFillColor(sf::Color(205, 127, 50));
            else
                entryText[i].setFillColor(sf::Color::White);
        }
        else
        {
            entryText[i].setString(inttostr(i + 1) + ".  ---");
            entryText[i].setFillColor(sf::Color(100, 100, 100));
        }
    }
}

void Leaderboardscreen::render(sf::RenderWindow& window)
{
    window.draw(bg);
    window.draw(title);
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++)
    {
        window.draw(rowBg[i]);
        window.draw(entryText[i]);
    }
    window.draw(closeBtn);
    window.draw(closeBtnText);
}

void Leaderboardscreen::handleEvent(sf::Event& e, bool& goBack)
{
    goBack = false;
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mp((float)e.mouseButton.x, (float)e.mouseButton.y);
        if (closeBtn.getGlobalBounds().contains(mp))
            goBack = true;
    }

    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Escape)
            goBack = true;
    }
}