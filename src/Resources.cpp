#include "Resources.hpp"
#include <stdexcept>
#include <boost/filesystem.hpp>

std::string Resources::String_Name = "Run 'n Gun";
sf::Font Resources::Font_Dosis;
sf::Texture Resources::Texture_Player;

std::vector<DataFile> Resources::Data_Weapons;
std::unordered_map<std::string, sf::Texture> Resources::Texture_Weapons;

void Resources::initialize()
{
    if (!Font_Dosis.loadFromFile("Dosis-Book.ttf")) throw std::runtime_error("Failed to load Dosis-Book.ttf");
    if (!Texture_Player.loadFromFile("Player.png")) throw std::runtime_error("Failed to load Player.png");

    for (auto it = boost::filesystem::directory_iterator("."); it != boost::filesystem::directory_iterator(); ++it)
    {
        std::string path = it->path().string();
        std::string ext = it->path().extension().string();

        if (it->path().extension() == ".weapon")
        {
            DataFile test;
            if (!test.loadFromFile(it->path().string())) throw std::runtime_error("Failed to load weapon from " + it->path().string());

            if (Texture_Weapons.count(test["Weapon Sprite"]) == 0) Texture_Weapons[test["Weapon Sprite"]];
            if (Texture_Weapons.count(test["Bullet Sprite"]) == 0) Texture_Weapons[test["Bullet Sprite"]];
            if (Texture_Weapons.count(test["Magazine Sprite"]) == 0) Texture_Weapons[test["Magazine Sprite"]];

            Data_Weapons.push_back(test);
        }
    }

    for (auto it = Texture_Weapons.begin(); it != Texture_Weapons.end(); ++it)
    {
        if (!it->second.loadFromFile(it->first)) throw std::runtime_error("Failed to load " + it->first);
    }
}