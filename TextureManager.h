#pragma once
#include <SFML/Graphics.hpp>

class Texturemanager
{
public:
    static Texturemanager& getinstance();

    bool loadAll();

    sf::Texture& getNick();
    sf::Texture& getPlayerBlue();
    sf::Texture& getPlayerGreen();
    sf::Texture& getPlayerRed();
    sf::Texture& getPlayerYellow();

    sf::Texture& getBotomBlue();
    sf::Texture& getBotomOrange();
    sf::Texture& getBotomPink();

    sf::Texture& getFoogaBlue();
    sf::Texture& getFoogaOrange();
    sf::Texture& getFoogaRed();

    sf::Texture& getTornadoBlue();
    sf::Texture& getTornadoRed();

    sf::Texture& getmogera();
    sf::Texture& getGamakichi();
    sf::Texture& getItems();

private:
    Texturemanager();
    Texturemanager(const Texturemanager&);
    Texturemanager& operator=(const Texturemanager&);

    sf::Texture nickTex;
    sf::Texture playerBlueTex;
    sf::Texture playerGreenTex;
    sf::Texture playerRedTex;
    sf::Texture playerYellowTex;

    sf::Texture botomBlueTex;
    sf::Texture botomOrangeTex;
    sf::Texture botomPinkTex;

    sf::Texture foogaBlueTex;
    sf::Texture foogaOrangeTex;
    sf::Texture foogaRedTex;

    sf::Texture tornadoBlueTex;
    sf::Texture tornadoRedTex;

    sf::Texture mogeraTex;
    sf::Texture gamakichiTex;
    sf::Texture itemsTex;

    bool loaded;
};