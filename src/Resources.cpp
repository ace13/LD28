#include "Resources.hpp"
#include <stdexcept>

sf::Font* Resources::Font_Dosis = nullptr;

void Resources::initialize()
{
    Font_Dosis = new sf::Font();
    if (!Font_Dosis->loadFromFile("Dosis-Book.ttf")) throw std::runtime_error("Failed to load Dosis-Book.ttf");
}