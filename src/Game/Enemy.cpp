#include "Enemy.hpp"
#include "Weapon.hpp"
#include "Dialog.hpp"
#include "Bullet.hpp"
#include "../Resources.hpp"
#include "../Math.hpp"
#include <tuple>
#include <random>
#include <SFML/Graphics/Sprite.hpp>
#include <Kunlaboro/EntitySystem.hpp>
#include <boost/algorithm/string/replace.hpp>

Enemy::Enemy() : Kunlaboro::Component("Game.Enemy"), mSheet(Resources::Texture_Enemy, 4, 2), mHealth(100), mArmor(1), mTime(0), mLastAng(0), mDrawAng(0), mFadeTime(0), mFear(false), mWeapon(nullptr), mInvulTime(0), mRandom(0)
{
}

Enemy::~Enemy() 
{
}

void Enemy::addedToEntity()
{
    auto getTaunt = [](Weapon* mWeapon, const std::string& type) -> std::string
    {
        std::random_device dev;
        std::uniform_int_distribution<int> dist(0, Resources::Data_Taunts[type].size()-1);
        
        std::string taunt = Resources::Data_Taunts[type][dist(dev)];
        if (mWeapon)
        {
            boost::algorithm::replace_all(taunt, "%b", mWeapon->bulletName());
            boost::algorithm::replace_all(taunt, "%w", mWeapon->weaponName());
        }
        else
        {
            boost::algorithm::replace_all(taunt, "%b", "nothings");
            boost::algorithm::replace_all(taunt, "%w", "Empty hands");
        }

        return taunt;
    };

    if (mPosition == sf::Vector2f())
    {
        sf::Vector2f playerPos;
        auto reply = sendGlobalQuestion("Where's the player?");
        if (reply.handled)
            playerPos = boost::any_cast<sf::Vector2f>(reply.payload);

        std::random_device dev;
        float randAng = std::uniform_real_distribution<float>(0, M_PI * 2)(dev);

        mPosition = playerPos + sf::Vector2f(cos(randAng), sin(randAng)) * 1024.f;

        mRandom = std::uniform_int_distribution<int>(0, 7)(dev);
    }

    requestMessage("Event.Update", [this, &getTaunt](const Kunlaboro::Message& msg)
    {
        float dt = boost::any_cast<float>(msg.payload);
        std::random_device dev;

        mInvulTime = std::max(0.f, mInvulTime - dt);

        sf::Vector2f playerPos;
        auto reply = sendGlobalQuestion("Where's the player?");
        if (reply.handled)
            playerPos = boost::any_cast<sf::Vector2f>(reply.payload);

        float playerAng = atan2(playerPos.y - mPosition.y, playerPos.x - mPosition.x);
        float randAng = std::uniform_real_distribution<float>(-0.1, 0.1)(dev);
        
        mLastAng += randAng;
        if (mFear)
            mLastAng += (playerAng+M_PI - mLastAng) * dt;
        else
            mLastAng += (playerAng - mLastAng) * dt;
        
        mPosition += sf::Vector2f(cos(mLastAng), sin(mLastAng)) * dt * 100.f;
        mDrawAng += (mLastAng - mDrawAng) / 8.f;

        mTime += dt;

        if (mTime > 1)
        {
            mTime -= 1;

            auto reply = sendQuestion("Such bullets?");
            if (!reply.handled)
                return;

            if (dynamic_cast<Weapon*>(reply.sender)->weaponType() == "Bonus")
            {
                sendMessage("Throw it to the ground!");
                return;
            }

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

            mLastFire = (playerAng + std::uniform_real_distribution<float>(-0.2, 0.2)(dev)) * (180/M_PI);

            sendMessage("Fire ze missiles!");
        }

        if (mHealth <= 25 && !mFear)
        {
            auto dialog = dynamic_cast<Dialog*>(getEntitySystem()->createComponent("Game.Dialog"));
            dialog->setMessage(getTaunt(nullptr, "lowhealth"));
            addLocalComponent(dialog);
            mFear = true;
        }

        if (mHealth <= 0)
        {
            sendMessage("Throw it to the ground!");

            getEntitySystem()->destroyEntity(getOwnerId());
        }
    });
    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload);
        auto& target = *std::get<0>(data);

        sf::FloatRect viewRect(target.getView().getCenter()+sf::Vector2f(32,32), target.getView().getSize()+sf::Vector2f(64,64));
        viewRect.left -= viewRect.width / 2.f;
        viewRect.top -= viewRect.height / 2.f;

        if (!viewRect.contains(mPosition))
        {
            mFadeTime += std::get<1>(data);

            if (mFadeTime > 10)
                getEntitySystem()->destroyEntity(getOwnerId());
        }
        else
        {
            mFadeTime = 0;

            sf::Sprite enemy(Resources::Texture_Enemy);
            enemy.setTextureRect(mSheet.getRect(mRandom % 4, mRandom / 4));
            enemy.setOrigin(enemy.getTextureRect().width / 2, enemy.getTextureRect().height / 2);
            enemy.setPosition(mPosition);
            enemy.setRotation(mDrawAng * (180 / M_PI));

            if (mInvulTime > 0)
                enemy.setColor(sf::Color::Red);

            target.draw(enemy);
        }
    });

    requestMessage("I'm ending this!", [this](const Kunlaboro::Message& msg) { getEntitySystem()->destroyEntity(getOwnerId()); });
    requestMessage("Where am I?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mPosition; }, true);
    requestMessage("Where am I shooting?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = std::make_tuple(mPosition, mLastFire); });
    requestMessage("Would the real slim shady please stand up?", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::list<Enemy*>*>(msg.payload);
        data->push_back(this);
    });
    requestMessage("Did I hit something?", [this, &getTaunt](Kunlaboro::Message& msg)
    {
        if (msg.sender->getOwnerId() == getOwnerId()) return;
        
        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        float diff = Math::distance(mPosition, pos);

        if (diff < 32)
        {
            auto& sys = *getEntitySystem();
            bool playerKill = !getEntitySystem()->getAllComponentsOnEntity(msg.sender->getOwnerId(), "Game.Player").empty();

            auto dialog = dynamic_cast<Dialog*>(sys.createComponent("Game.Dialog"));
            dialog->setMessage(getTaunt(nullptr, playerKill ? "hit" : "friendlyfire"));
            addLocalComponent(dialog);

            auto bullet = dynamic_cast<Bullet*>(msg.sender);
            float lastHealth = mHealth;
            mHealth -= bullet->getDamage();

            if (mHealth <= 0 && lastHealth > 0)
            {
                sendGlobalMessage("Is dead now", playerKill);
            }
            else if (mInvulTime == 0)
            {
                mInvulTime = 0.2;
                mPosition += (mPosition - pos)/diff * 8.f;
            }

            msg.payload = true;
            msg.handled = true;
        }
    });

    requestComponent("Game.Weapon", [this, &getTaunt](const Kunlaboro::Message& msg)
    {
        mWeapon = dynamic_cast<Weapon*>(msg.sender);

        if (msg.type == Kunlaboro::Type_Create)
        {
            auto& sys = *getEntitySystem();

            {
                auto dialog = dynamic_cast<Dialog*>(sys.createComponent("Game.Dialog"));
                dialog->setMessage(getTaunt(mWeapon, "pickup"));
                addLocalComponent(dialog);
            }
        }
        else
        {
            auto& sys = *getEntitySystem();

            auto dialog = dynamic_cast<Dialog*>(sys.createComponent("Game.Dialog"));
            dialog->setMessage(getTaunt(mWeapon, "empty"));
            addLocalComponent(dialog);

            mFear = true;

            mWeapon = nullptr;
        }
    });
}