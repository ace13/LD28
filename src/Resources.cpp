#include "Resources.hpp"
#include <stdexcept>

std::string Resources::String_Name = "Run 'n Gun";
sf::Font* Resources::Font_Dosis = nullptr;
sf::Texture* Resources::Texture_Player = nullptr;
std::list<DataFile> Resources::File_Data;

void Resources::initialize()
{
    Font_Dosis = new sf::Font();
    if (!Font_Dosis->loadFromFile("Dosis-Book.ttf")) throw std::runtime_error("Failed to load Dosis-Book.ttf");
    Texture_Player = new sf::Texture();
    if (!Texture_Player->loadFromFile("Player.png")) throw std::runtime_error("Failed to load Player.png");
    
    DataFile test;
    test.loadFromFile("test.weapon");
    File_Data.push_back(test);
}