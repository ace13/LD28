#pragma once

#include "DataFile.hpp"
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

struct Resources
{

static std::string String_Name;

static sf::Font Font_Dosis;
static sf::Texture Texture_Player;
static sf::Texture Texture_Enemy;
static sf::Texture Texture_Ground;

static std::vector<DataFile> Data_Weapons;
static std::unordered_map<std::string, sf::Texture> Texture_Weapons;

static void initialize();
};
