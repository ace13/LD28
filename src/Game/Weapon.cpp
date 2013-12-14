#include "Weapon.hpp"
#include "../Resources.hpp"
#include <random>

Weapon::Weapon() : Kunlaboro::Component("Game.Weapon")
{
    std::random_device dev;

    std::uniform_int_distribution<int> weapon(0, Resources::File_Data.size() - 1);

    DataFile& file = Resources::File_Data[weapon(dev)];

    mName = file["Name"];
    mDamage = std::uniform_real_distribution<float>(atof(file["Min Damage"].c_str()), atof(file["Max Damage"].c_str()))(dev);
    mSpread = std::uniform_real_distribution<float>(atof(file["Min Spread"].c_str()), atof(file["Max Spread"].c_str()))(dev);
    mFireRate = std::uniform_real_distribution<float>(atof(file["Min Rate"].c_str()), atof(file["Max Rate"  ].c_str()))(dev);
    
}

Weapon::~Weapon()
{
}

void Weapon::addedToEntity()
{

}
