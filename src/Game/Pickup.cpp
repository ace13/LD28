#include "Pickup.hpp"
#include "Weapon.hpp"
#include "../Math.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Kunlaboro/EntitySystem.hpp>
#include <random>
#include <cmath>

Pickup::Pickup() : Kunlaboro::Component("Game.Pickup"), mAngle(0), mTexture(nullptr), mLifeTime(0)
{
}

Pickup::~Pickup()
{
}

void Pickup::addedToEntity()
{
    {
        std::random_device dev;

        mAngle = std::uniform_real_distribution<float>(0, M_PI*2)(dev);
        mInertia = std::uniform_real_distribution<float>(64, 256)(dev);

        mStartAngle = mAngle;
    }

    requestMessage("Event.Update", [this](const Kunlaboro::Message& msg)
    {
        float dt = boost::any_cast<float>(msg.payload);

        if (mInertia > 0)
        {
            mPosition += sf::Vector2f(cos(mStartAngle), sin(mStartAngle)) * mInertia * dt;
            mAngle += mInertia * dt * (M_PI/180);
            mInertia -= dt * 100;
        }
        else
        {
            mLifeTime += dt;

            if (mLifeTime > 10)
                getEntitySystem()->destroyEntity(getOwnerId());
        }
    });

    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        if (!mTexture)
            return;

        auto& target = *std::get<0>(boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload));

        sf::Sprite sprite(*mTexture);
        sprite.setOrigin((sf::Vector2f)mTexture->getSize() / 2.f);
        sprite.setPosition(mPosition);
        sprite.setRotation(mAngle * (180 / M_PI));

        if (mLifeTime > 6)
        {
            if ((int)(mLifeTime*2) % 2)
                sprite.setColor(sf::Color::Red);

            float perc = 1 - (mLifeTime-6)/4;
            sprite.setScale(perc, perc);
        }

        target.draw(sprite);
    });
    changeRequestPriority("Event.Draw", -1);

    requestComponent("Game.Weapon", [this](const Kunlaboro::Message& msg)
    {
        if (msg.type == Kunlaboro::Type_Create)
            mTexture = &dynamic_cast<Weapon*>(msg.sender)->weaponTexture();
        else
        {
            mTexture = nullptr;
            getEntitySystem()->destroyEntity(getOwnerId());
        }
    });

    requestMessage("Mr grabbyhands", [this](Kunlaboro::Message& msg)
    {
        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);

        if (Math::distance(pos, mPosition) < 64)
        {
            msg.payload = true;
            msg.handled = true;
        }
    });

    requestMessage("I'm ending this!", [this](const Kunlaboro::Message& msg) { getEntitySystem()->destroyEntity(getOwnerId()); });
}
