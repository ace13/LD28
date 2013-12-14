#include "Enemy.hpp"
#include "Weapon.hpp"
#include "Dialog.hpp"
#include "../Resources.hpp"
#include <tuple>
#include <random>
#include <SFML/Graphics/Sprite.hpp>
#include <Kunlaboro/EntitySystem.hpp>

Enemy::Enemy() : Kunlaboro::Component("Game.Enemy"), mSheet(Resources::Texture_Enemy, 4, 2), mPosition(100, 100), mHealth(100), mArmor(1), mTime(0), mLastAng(0)
{
}

Enemy::~Enemy() 
{
}

void Enemy::addedToEntity()
{
    requestMessage("Event.Update", [this](const Kunlaboro::Message& msg)
    {
        float dt = boost::any_cast<float>(msg.payload);
        std::random_device dev;

        float randAng = std::uniform_real_distribution<float>(-0.1, 0.1)(dev);
        mLastAng += randAng;
        mPosition += sf::Vector2f(cos(mLastAng), sin(mLastAng)) * dt * 100.f;

        mTime += dt;

        if (mTime > 1)
        {
            mTime -= 1;

            auto reply = sendQuestion("Such bullets?");
            if (!reply.handled)
                return;

            int ammo = boost::any_cast<int>(reply.payload);

            if (ammo == 0)
            {
                sendMessage("More ammo!");

                auto reply = sendQuestion("Out of mags?");
                int mags = boost::any_cast<int>(reply.payload);;

                if (mags == 0)
                {
                    sendMessage("Throw it to the ground!");
                    return;
                }
            }

            mLastFire = std::uniform_real_distribution<float>(0, 360)(dev);

            sendMessage("Fire ze missiles!");
        }
    });
    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        auto& target = *boost::any_cast<sf::RenderTarget*>(msg.payload);

        sf::Sprite enemy(Resources::Texture_Enemy);
        enemy.setTextureRect(mSheet.getRect(0,0));
        enemy.setOrigin(enemy.getTextureRect().width / 2, enemy.getTextureRect().height / 2);
        enemy.setPosition(mPosition);
        enemy.setRotation(mLastAng * (180 / 3.1415));

        target.draw(enemy);
    });

    requestMessage("Where am I?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mPosition; }, true);
    requestMessage("Where am I shooting?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = std::make_tuple(mPosition, mLastFire); });

    requestComponent("Game.Weapon", [this](const Kunlaboro::Message& msg)
    {
        auto weap = dynamic_cast<Weapon*>(msg.sender);

        if (msg.type == Kunlaboro::Type_Create)
        {
            auto& sys = *getEntitySystem();

            auto dialog = dynamic_cast<Dialog*>(sys.createComponent("Game.Dialog"));
            dialog->setMessage("Eat " + weap->weaponName() + " and die!");
            addLocalComponent(dialog);
        }
        else
        {
            auto& sys = *getEntitySystem();

            auto dialog = dynamic_cast<Dialog*>(sys.createComponent("Game.Dialog"));
            dialog->setMessage("Aah! No more " + weap->weaponName()+ "\nRUN AWAY!");
            addLocalComponent(dialog);
        }
    });
}