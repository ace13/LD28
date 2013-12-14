#include "Weapon.hpp"
#include "../Resources.hpp"
#include <random>
#include <tuple>
#include <SFML/Window/Mouse.hpp>

Weapon::Weapon() : Kunlaboro::Component("Game.Weapon")
{
    
}

Weapon::~Weapon()
{
}

void Weapon::addedToEntity()
{
    { // Weapon Setup
        std::random_device dev;

        std::uniform_int_distribution<int> weapon(0, Resources::Data_Weapons.size() - 1);

        DataFile& file = Resources::Data_Weapons[weapon(dev)];

        mName = file["Name"];
        mDamage = std::uniform_real_distribution<float>(atof(file["Min Damage"].c_str()), atof(file["Max Damage"].c_str()))(dev);
        mSpread = std::uniform_real_distribution<float>(atof(file["Min Spread"].c_str()), atof(file["Max Spread"].c_str()))(dev);
        mFireRate = std::uniform_real_distribution<float>(atof(file["Min Rate"].c_str()), atof(file["Max Rate"  ].c_str()))(dev);
    
        mMags = std::uniform_int_distribution<int>(atoi(file["Min Mags"].c_str()), atoi(file["Max Mags"].c_str()))(dev);
        mBulletsPerMag = std::uniform_int_distribution<int>(atoi(file["Min Bullets"].c_str()), atoi(file["Max Bullets"].c_str()))(dev);
        mBulletsPerShot = atoi(file["Bullets Per Shot"].c_str());

        mBulletsInCurrentMag = mBulletsPerMag;

        mTexture = &Resources::Texture_Weapons[file["Weapon Sprite"]];
        mBulletTexture = &Resources::Texture_Weapons[file["Bullet Sprite"]];
        mMagazineTexture = &Resources::Texture_Weapons[file["Magazine Sprite"]];
    }

    requestMessage("Event.Mouse.Click", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::Mouse::Button, sf::Vector2f, bool> >(msg.payload);

        if (std::get<0>(data) == sf::Mouse::Left && std::get<2>(data))
        {
            if (mBulletsInCurrentMag > 0)
                --mBulletsInCurrentMag;
        }
    });

    requestMessage("Event.Key.R", [this](const Kunlaboro::Message& msg)
    {
        auto pressed = boost::any_cast<bool>(msg.payload);

        if (pressed && mMags > 0)
        {
            mBulletsInCurrentMag = mBulletsPerMag;
            --mMags;
        }   
    });

}
