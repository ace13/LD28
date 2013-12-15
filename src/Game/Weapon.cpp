#include "Weapon.hpp"
#include "Pickup.hpp"
#include "../Resources.hpp"
#include <random>
#include <tuple>
#include <SFML/Window/Mouse.hpp>
#include <Kunlaboro/EntitySystem.hpp>

Weapon::Weapon() : Kunlaboro::Component("Game.Weapon"), mTimeout(0)
{
    
}

Weapon::~Weapon()
{
}

void Weapon::addedToEntity()
{
    if (mName.empty())
    { // Weapon Setup
        std::random_device dev;

        DataFile& file = Resources::Data_Weapons[std::uniform_int_distribution<int>(0, Resources::Data_Weapons.size() - 1)(dev)];

        mName = file["Name"];
        mType = file["Type"];
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
        if (mBulletsInCurrentMag > 0 && mTimeout == 0)
        {
            //mTimeout = mFireRate;
            --mBulletsInCurrentMag;

            if (mType == "Bonus") return;

            for (int i = 0; i < mBulletsPerShot; ++i)
            {
                getEntitySystem()->addComponent(getOwnerId(), "Game.Bullet");
            }
        }
    }, true);

    requestMessage("More ammo!", [this](const Kunlaboro::Message& msg)
    {
        if (mType == "Bonus") return;

        if (mMags > 0)
        {
            if (mBulletsInCurrentMag > 0)
            {
                printf("TODO: particles for lost ammo!\n");
            }

            mBulletsInCurrentMag = mBulletsPerMag;
            --mMags;
        }   
    }, true);

    requestMessage("Throw it to the ground!", [this](Kunlaboro::Message& msg)
    {
        auto reply = sendQuestion("Where am I?");
        if (!reply.handled)
            return;

        auto& sys = *getEntitySystem();

        auto ent = sys.createEntity();
        auto pickup = dynamic_cast<Pickup*>(sys.createComponent("Game.Pickup"));
        pickup->setPosition(boost::any_cast<sf::Vector2f>(reply.payload));

        sys.addComponent(ent, pickup);
        
        sys.removeComponent(getOwnerId(), this);
        sys.addComponent(ent, this);        

        msg.handled = true;
    }, true);

    requestMessage("What am I holding?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mName; }, true);
    requestMessage("Such bullets?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mBulletsInCurrentMag; }, true);
    requestMessage("Out of mags?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mMags; }, true);
}
