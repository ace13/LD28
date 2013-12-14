#pragma once

#include "DataFile.hpp"
#include <list>
#include <SFML/Graphics.hpp>

struct Resources
{

static std::string String_Name;

static sf::Font* Font_Dosis;
static sf::Texture* Texture_Player;

static std::list<DataFile> File_Data;


static void initialize();
};
