#include "Resources.hpp"
#include <stdexcept>

std::string Resources::String_Name = "Run 'n Gun";
sf::Font* Resources::Font_Dosis = nullptr;
sf::Texture* Resources::Texture_Player = nullptr;
std::vector<DataFile> Resources::File_Data;

void Resources::initialize()
{
    Font_Dosis = new sf::Font();
    if (!Font_Dosis->loadFromFile("Dosis-Book.ttf")) throw std::runtime_error("Failed to load Dosis-Book.ttf");
    Texture_Player = new sf::Texture();
    if (!Texture_Player->loadFromFile("Player.png")) throw std::runtime_error("Failed to load Player.png");
    
    const std::string DataFiles[] = { "test.weapon" };

    for (int i = 0; i < sizeof(DataFiles) / sizeof(std::string); ++i)
    {
        DataFile test;
        test.loadFromFile(DataFiles[i]);
        File_Data.push_back(test);
    }
}