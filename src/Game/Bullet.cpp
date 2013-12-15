#include "Bullet.hpp"
#include "Weapon.hpp"
#include "Player.hpp"
#include "../Math.hpp"
#include <Kunlaboro/EntitySystem.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <random>

const float gBulletSpeed = 1024;

Bullet::Bullet() : Kunlaboro::Component("Game.Bullet"), mDamage(0), mAngle(0), mLifeTime(-1)
{

}

Bullet::~Bullet()
{

}

void Bullet::addedToEntity()
{
    float spread;
    { // Read weapon data
        auto weapon = dynamic_cast<Weapon*>(getEntitySystem()->getAllComponentsOnEntity(getOwnerId(), "Game.Weapon")[0]);
        mTexture = &weapon->bulletTexture();
        mDamage = weapon->getDamage();
        spread = weapon->getSpread();
    }

    { // Read player data
        auto reply = sendQuestion("Where am I shooting?");
        if (!reply.handled)
        {
            getEntitySystem()->destroyComponent(this);
            return;
        }

        auto data = boost::any_cast<std::tuple<sf::Vector2f, float> >(reply.payload);

        mPosition = std::get<0>(data);
        mAngle = std::get<1>(data) * (M_PI / 180);

        std::random_device dev;
        std::uniform_real_distribution<float> rs(-spread, spread);

        mAngle += rs(dev) * (M_PI / 180);
    }

    requestMessage("Event.Update", [this](const Kunlaboro::Message& msg)
    {
        float dt = boost::any_cast<float>(msg.payload);

        mPosition += sf::Vector2f(cos(mAngle), sin(mAngle)) * dt * gBulletSpeed;

        mLifeTime += dt;
        if (mLifeTime > 2.f)
            getEntitySystem()->destroyComponent(this);

        auto reply = sendGlobalQuestion("Did I hit something?", mPosition);
        if (reply.handled)
        {
            getEntitySystem()->destroyComponent(this);
        }
    });

    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        auto& target = *std::get<0>(boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload));

        sf::FloatRect rect;
        {
            sf::Vector2f vC = target.getView().getCenter(), vS = target.getView().getSize();
            rect.left = vC.x - vS.x / 2.f;
            rect.top = vC.y - vS.y / 2.f;
            rect.width = vS.x;
            rect.height = vS.y;
        }

        sf::Sprite bullet(*mTexture);
        bullet.setPosition(mPosition);
        bullet.setOrigin(mTexture->getSize().x / 2.f, mTexture->getSize().y / 2.f);
        bullet.setRotation((mAngle + 90) * (180 / M_PI));

        bool flip = (int)(mLifeTime * 6) % 2;

        if (flip)
            bullet.setScale(-1, 1);

        const int trailLength = 6;
        auto size = mTexture->getSize();
        float sizeLen = (float)sqrt((float)size.x*size.x + size.y*size.y) / 1.618f;

        sf::Vector2f dir = sf::Vector2f(cos(mAngle), sin(mAngle));
        bullet.move(dir * (float)-trailLength * sizeLen);

        for (int i = 0; i < trailLength; ++i)
        {
            float str = 255 * i/6;
            bullet.setColor(sf::Color(str, str, str, str));
            target.draw(bullet);

            bullet.move(dir * sizeLen);
        }

        target.draw(bullet);
    });
}