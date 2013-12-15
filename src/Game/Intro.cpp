#include "Intro.hpp"
#include "../Resources.hpp"
#include <Kunlaboro/EntitySystem.hpp>
#include <tuple>

const float gIntroTime = 5;

Intro::Intro() : Kunlaboro::Component("Game.Intro"), mIntroTime(0)
{
}

Intro::~Intro()
{
}

void Intro::addedToEntity()
{
    if (!mTexture.loadFromFile("Ace.png")) throw std::runtime_error("Failed to load Ace.png");
    mTexture.setSmooth(true);

    requestMessage("Event.Update", [this](const Kunlaboro::Message&)
    {
        if (mIntroTime > gIntroTime)
        {
            auto ent = getEntitySystem()->createEntity();
            getEntitySystem()->addComponent(ent, "Game.Menu");

            getEntitySystem()->destroyEntity(getOwnerId());
        }
    });

    requestMessage("Event.DrawUi", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::RenderTarget*, float>>(msg.payload);
        auto& target = *std::get<0>(data);

        mIntroTime += std::get<1>(data);

        sf::Sprite sprite(mTexture);
        sprite.setOrigin((sf::Vector2f)mTexture.getSize() / 2.f);
        sprite.setPosition(target.getView().getCenter());
        sprite.setRotation(mIntroTime - 5);

        float scale = std::min(1.f, (mIntroTime*mIntroTime));

        if (scale == 1.f)
        {
            scale += mIntroTime;
            sprite.setScale(scale, scale);
            sprite.setColor(sf::Color(255,255,255,64 - mIntroTime*12.9));
            target.draw(sprite);

            scale = 1;
        }

        sprite.setColor(sf::Color::White);
        sprite.setScale(scale, scale);

        target.draw(sprite);

        sf::Text naic("An Ace game", Resources::Font_Dosis, 30);
        naic.setOrigin(naic.getLocalBounds().width/2.f, naic.getLocalBounds().height/2.f);
        naic.setPosition(target.getView().getSize().x / 2.f, target.getView().getSize().y - 32);
        naic.setColor(sf::Color(0,0,0,96));

        naic.move(-1, -1);
        target.draw(naic);
        naic.move(2, 0);
        target.draw(naic);
        naic.move(0, 2);
        target.draw(naic);
        naic.move(-2, 0);
        target.draw(naic);

        naic.setPosition(target.getView().getSize().x / 2.f, target.getView().getSize().y - 32);
        naic.setColor(sf::Color::White);

        target.draw(naic);
    });
}
