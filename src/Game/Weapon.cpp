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

        DataFile& file = Resources::Data_Weapons[std::uniform_int_distribution<int>(0, Resources::Data_Weapons.size() - 1)(dev)];

        mName = file["Name"];
        mBulletName = file["Bullet Name"];
        mDamage = atof(file["Damage"].c_str());
        mSpread = atof(file["Spread"].c_str());
        mFireRate = atof(file["Rate"].c_str());
        mReloadTime = atof(file["Reload Time"].c_str());

        mMags = atoi(file["Mags"].c_str());
        mBulletsPerMag = atoi(file["Bullets"].c_str());
        mBulletsPerShot = atoi(file["Bullets Per Shot"].c_str());

        mBulletsInCurrentMag = std::uniform_int_distribution<int>(0, mBulletsPerMag)(dev);

        mTexture = &Resources::Texture_Weapons[file["Weapon Sprite"]];
        mBulletTexture = &Resources::Texture_Weapons[file["Bullet Sprite"]];
        mMagazineTexture = &Resources::Texture_Weapons[file["Magazine Sprite"]];
    }

    requestMessage("Fire ze missiles!", [this](const Kunlaboro::Message& msg)
    {
        if (mBulletsInCurrentMag > 0)
        {
            --mBulletsInCurrentMag;

            for (int i = 0; i < mBulletsPerShot; ++i)
            {
                getEntitySystem()->addComponent(getOwnerId(), "Game.Bullet");
            }
        }
    }, true);

    requestMessage("More ammo!", [this](const Kunlaboro::Message& msg)
    {
        if (mMags > 0)
        {
            mBulletsInCurrentMag = mBulletsPerMag;
            --mMags;
        }   
    }, true);

    requestMessage("Throw it to the ground!", [this](const Kunlaboro::Message& msg)
    {
        ///\TODO Throw the weapon
        printf("TODO: Throw weapon TO THE GROUND\n");
        getEntitySystem()->destroyComponent(this);
    }, true);

    requestMessage("What am I holding?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mName; }, true);
    requestMessage("Such bullets?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mBulletsInCurrentMag; }, true);
    requestMessage("Out of mags?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mMags; }, true);
}
