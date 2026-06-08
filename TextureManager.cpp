#include "TextureManager.h"
#include <iostream>

Texturemanager::Texturemanager()
{
    loaded = false;
}

Texturemanager& Texturemanager::getinstance()
{
    static Texturemanager instance;
    return instance;
}

bool Texturemanager::loadAll()
{
    std::string base = "Assets/Images/";

    if (!nickTex.loadFromFile(base + "Nick.png")) return false;
    if (!playerBlueTex.loadFromFile(base + "Player_Blue.png")) return false;
    if (!playerGreenTex.loadFromFile(base + "Player_Green.png")) return false;
    if (!playerRedTex.loadFromFile(base + "Player_Red.png")) return false;
    if (!playerYellowTex.loadFromFile(base + "Player_Yellow.png")) return false;

    if (!botomBlueTex.loadFromFile(base + "Botom_Blue.png")) return false;
    if (!botomOrangeTex.loadFromFile(base + "Botom_Orange.png")) return false;
    if (!botomPinkTex.loadFromFile(base + "Botom_Pink.png")) return false;

    if (!foogaBlueTex.loadFromFile(base + "FlyingFoogaFoog_Blue.png")) return false;
    if (!foogaOrangeTex.loadFromFile(base + "FlyingFoogaFoog_Orange.png")) return false;
    if (!foogaRedTex.loadFromFile(base + "FlyingFoogaFoog_Red.png")) return false;

    if (!tornadoBlueTex.loadFromFile(base + "Tornado_Blue.png")) return false;
    if (!tornadoRedTex.loadFromFile(base + "Tornado_Red.png")) return false;

    if (!mogeraTex.loadFromFile(base + "Mogera.png")) return false;
    if (!gamakichiTex.loadFromFile(base + "Gamakichi.png")) return false;
    if (!itemsTex.loadFromFile(base + "Items.png")) return false;

    loaded = true;
    return true;
}

sf::Texture& Texturemanager::getNick() { return nickTex; }
sf::Texture& Texturemanager::getPlayerBlue() { return playerBlueTex; }
sf::Texture& Texturemanager::getPlayerGreen() { return playerGreenTex; }
sf::Texture& Texturemanager::getPlayerRed() { return playerRedTex; }
sf::Texture& Texturemanager::getPlayerYellow() { return playerYellowTex; }
sf::Texture& Texturemanager::getBotomBlue() { return botomBlueTex; }
sf::Texture& Texturemanager::getBotomOrange() { return botomOrangeTex; }
sf::Texture& Texturemanager::getBotomPink() { return botomPinkTex; }
sf::Texture& Texturemanager::getFoogaBlue() { return foogaBlueTex; }
sf::Texture& Texturemanager::getFoogaOrange() { return foogaOrangeTex; }
sf::Texture& Texturemanager::getFoogaRed() { return foogaRedTex; }
sf::Texture& Texturemanager::getTornadoBlue() { return tornadoBlueTex; }
sf::Texture& Texturemanager::getTornadoRed() { return tornadoRedTex; }
sf::Texture& Texturemanager::getmogera() { return mogeraTex; }
sf::Texture& Texturemanager::getGamakichi() { return gamakichiTex; }
sf::Texture& Texturemanager::getItems() { return itemsTex; }