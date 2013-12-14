#pragma once

#include "DataFile.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

struct Resources
{

static std::string String_Name;

static sf::Font* Font_Dosis;
static sf::Texture* Texture_Player;

static std::vector<DataFile> File_Data;


static void initialize();
};
