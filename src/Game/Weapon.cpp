#include "Weapon.hpp"
#include "../Resources.hpp"
#include <random>
#include <tuple>
#include <SFML/Window/Mouse.hpp>
#include <Kunlaboro/EntitySystem.hpp>

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
        mDamage = atof(file["Damage"].c_str());
        mSpread = atof(file["Spread"].c_str());
        mFireRate = atof(file["Rate"].c_str());
    
        mMags = atoi(file["Mags"].c_str());
        mBulletsPerMag = atoi(file["Bullets"].c_str());
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
            {
                --mBulletsInCurrentMag;

                for (int i = 0; i < mBulletsPerShot; ++i)
                {
                    getEntitySystem()->addComponent(getOwnerId(), "Game.Bullet");
                }
            }
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

    requestMessage("Event.Key.Q", [this](const Kunlaboro::Message& msg)
    {
        if (!boost::any_cast<bool>(msg.payload)) return;

        ///\TODO Throw the weapon
        printf("TODO: Throw weapon\n");
        getEntitySystem()->destroyComponent(this);
    });

}
