#include "Player.hpp"
#include "Weapon.hpp"
#include "Dialog.hpp"
#include "Bullet.hpp"
#include "../Math.hpp"
#include "../Resources.hpp"

#include <Kunlaboro/EntitySystem.hpp>

const float gMoveSpeed = 256;
const float gCameraFloat = 1024;

enum Move
{
    M_W = 1,
    M_A = 2,
    M_S = 4,
    M_D = 8,
    M_Shift = 16
};

Player::Player() : Kunlaboro::Component("Game.Player"), mSheet(Resources::Texture_Player, 4, 2), mTime(0), mPressed(0), mWeapon(nullptr), mLastWalkAng(-1000), mHealth(100)
{
}

Player::~Player()
{
}

void Player::addedToEntity()
{
    sf::View* gameView = nullptr;
    auto response = sendGlobalQuestion("Get.GameView");
    if (response.handled)
        gameView = boost::any_cast<sf::View*>(response.payload);

    requestMessage("Is the game running?", [](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = true; });
    requestMessage("I'm ending this!", [this](const Kunlaboro::Message& msg) { getEntitySystem()->destroyEntity(getOwnerId()); });

    requestMessage("Event.Update", [this, gameView](const Kunlaboro::Message& msg)
    {
        float dt = boost::any_cast<float>(msg.payload);

        mHealth = std::min(100.f, mHealth + dt);

        auto curPos = gameView->getCenter();
        gameView->move((mPosition - curPos) * dt * 2.f);

        sf::Vector2f diff((mPressed & M_D)/M_D - (mPressed & M_A)/M_A, (mPressed & M_S)/M_S - (mPressed & M_W)/M_W);
        float len = std::min((float)sqrt(diff.x * diff.x + diff.y * diff.y), 1.f);

        if (len > 0.5)
        {
            float ang = atan2(diff.y, diff.x);
            mLastWalkAng = ang * (180 / M_PI);
            diff = sf::Vector2f(cos(ang), sin(ang)) * len;

            float boost = 1;
            if (mWeapon && mWeapon->weaponType() == "Bonus")
                boost = mWeapon->getRate();

            mPosition += diff * dt * gMoveSpeed * (1.f + (mPressed & M_Shift)/M_Shift) * boost;
        }
        else
            mLastWalkAng = -1000;

        mTime += dt * 2;
    });
    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        auto& target = *std::get<0>(boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload));

        sf::Sprite sprite(Resources::Texture_Player);
        sprite.setTextureRect(mSheet.getRect(0,0));
        sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
        sprite.setPosition(mPosition);

        if (mLastWalkAng > -1000)
        {
            bool sprint = (mPressed & M_Shift);
            
            sprite.setRotation(mLastWalkAng + 90); 
            if (sprint)
            {
                int val = (int)(mTime * 4) % 4;
                sprite.setTextureRect(mSheet.getRect((val < 2 ? (val % 2) * 2 : 1 + (val % 2) * 2), 1));
                target.draw(sprite);
            }
            else
            {
                int val = (int)(mTime) % 4;
                sprite.setTextureRect(mSheet.getRect(val/2, 1));
                if (val % 2)
                    target.draw(sprite);
            }
        }

        sprite.setTextureRect(mSheet.getRect((int)mTime % 4, 0));
        sprite.setRotation(mAngle + 90);
        target.draw(sprite);
    });

    requestMessage("Event.DrawUi", [this](const Kunlaboro::Message& msg)
    {
        auto& target = *std::get<0>(boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload));

        if (mWeapon)
        {
            auto size = target.getView().getSize();
            size.y = 96;
            sf::RectangleShape shape(size);
            shape.setFillColor(sf::Color(0,0,0, 96));
            target.draw(shape);

            sf::Sprite weap(mWeapon->weaponTexture());
            weap.move(5, 5);
            target.draw(weap);

            sf::Text weapName(mWeapon->weaponName(), Resources::Font_Dosis);
            weapName.move(weap.getTexture()->getSize().x + 10, 5);
            target.draw(weapName);

            weap.move(mWeapon->weaponTexture().getSize().x + 10, mWeapon->weaponTexture().getSize().y);
            weap.move(0, mWeapon->magazineTexture().getSize().y / -3.f);
            weap.setTexture(mWeapon->magazineTexture(), true);

            for (int i = 0; i < mWeapon->magazinesLeft(); ++i)
            {
                target.draw(weap);

                weap.move(mWeapon->magazineTexture().getSize().x + 4, 0);
            }

            weap.setTexture(mWeapon->bulletTexture(), true);

            for (int i = 0; i < mWeapon->bulletsLeft(); ++i)
            {
                target.draw(weap);

                weap.move(mWeapon->bulletTexture().getSize().x + 2, 0);
            }
        }
    });

    requestComponent("Game.Weapon", [this](const Kunlaboro::Message& msg)
    {
        if (msg.type == Kunlaboro::Type_Create)
            mWeapon = dynamic_cast<Weapon*>(msg.sender);
        else
        {
            mWeapon = nullptr;
        }
    });

    requestMessage("Where am I shooting?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = std::make_tuple(mPosition, mAngle); }, true);
    requestMessage("Where am I?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mPosition; }, true);
    requestMessage("Where's the player?", [this](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = mPosition; });

    requestMessage("Event.Key.W", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_W; else mPressed &= ~M_W; });
    requestMessage("Event.Key.A", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_A; else mPressed &= ~M_A; });
    requestMessage("Event.Key.S", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_S; else mPressed &= ~M_S; });
    requestMessage("Event.Key.D", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_D; else mPressed &= ~M_D; });
    requestMessage("Event.Key.LShift", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_Shift; else mPressed &= ~M_Shift; });
    requestMessage("Event.Key.E", [this](const Kunlaboro::Message& msg)
    {
        if (!boost::any_cast<bool>(msg.payload)) return;

        if (!mWeapon)
        {
            auto reply = sendGlobalQuestion("Mr grabbyhands", mPosition);
            if (reply.handled)
            {
                auto weapon = dynamic_cast<Weapon*>(getEntitySystem()->getAllComponentsOnEntity(reply.sender->getOwnerId(), "Game.Weapon")[0]);
                
                getEntitySystem()->removeComponent(reply.sender->getOwnerId(), weapon);
                addLocalComponent(weapon);
            }
        }
    });
    requestMessage("Event.Key.Q", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) sendMessage("Throw it to the ground!"); });
    requestMessage("Event.Key.R", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) sendMessage("More ammo!"); });

    requestMessage("Event.Mouse.MoveGame", [this](const Kunlaboro::Message& msg)
    {
        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        mMouse = pos;

        float ang = atan2(pos.y - mPosition.y, pos.x - mPosition.x);

        mAngle = ang * (180.f/3.1415f);
    });

    requestMessage("Event.Mouse.Click", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::Mouse::Button, sf::Vector2f, bool> >(msg.payload);

        if (std::get<0>(data) == sf::Mouse::Left && std::get<2>(data))
            sendMessage("Fire ze missiles!");
    });

    requestMessage("Event.Key.Escape", [this](const Kunlaboro::Message& msg)
    {
        bool pressed = boost::any_cast<bool>(msg.payload);
        if (!pressed)
            return;

        auto response = sendGlobalQuestion("Is the game paused?");

        if (!response.handled || !boost::any_cast<bool>(response.payload))
        {
            auto& sys = *getEntitySystem();
            auto ent = sys.createEntity();
            sys.addComponent(ent, "Game.Menu");
        }
    });

    requestMessage("Did I hit something?", [this](Kunlaboro::Message& msg)
    {
        if (msg.sender->getOwnerId() == getOwnerId()) return; 
        
        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        float diff = Math::distance(mPosition, pos);

        if (diff < 32)
        {
            auto bullet = dynamic_cast<Bullet*>(msg.sender);
            float damage = bullet->getDamage();

            if (mWeapon && mWeapon->weaponType() == "Bonus" && mWeapon->magazinesLeft() > 0)
            {
                damage *= mWeapon->getDamage();
                mWeapon->throwMagazine();
            }

            {
                auto dialog = dynamic_cast<Dialog*>(getEntitySystem()->createComponent("Game.Dialog"));
                if (damage < 0.1)
                    dialog->setMessage("Didn't hurt");
                else
                {
                    dialog->setMessage("OW");

                    mHealth -= damage;
                }
                addLocalComponent(dialog);
            }
            
            msg.payload = true;
            msg.handled = true;
        }
    });
}
