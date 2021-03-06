#include "Dialog.hpp"
#include "../Resources.hpp"
#include <Kunlaboro/EntitySystem.hpp>

const float gDistance = 96;
const float gHorizDistance = 128;

Dialog::Dialog(): Kunlaboro::Component("Game.Dialog"), mTime(0), mPosition(-10000, 10000)
{
}

Dialog::~Dialog()
{

}

void Dialog::addedToEntity()
{
    auto has = getEntitySystem()->getAllComponentsOnEntity(getOwnerId(), "Game.Dialog").size();
    if (has > 1)
        getEntitySystem()->destroyComponent(this);
    else
    {
        requestMessage("Event.Update", [this](const Kunlaboro::Message& msg)
        { 
            float dt = boost::any_cast<float>(msg.payload);

            mTime += dt;

            auto reply = sendQuestion("Where am I?");
            if (reply.handled)
            {
                mPosition = boost::any_cast<sf::Vector2f>(reply.payload);
            }

            if (mTime > 5.f)
            {
                getEntitySystem()->destroyComponent(this);
            }
        });

        requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
        {
            auto& target = *std::get<0>(boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload));

            sf::Text message(mMessage, Resources::Font_Dosis, 26U);
            auto rect = message.getLocalBounds();
            sf::RectangleShape box(sf::Vector2f(rect.width + 16, rect.height + 16));
            sf::ConvexShape speechThingy(3);

            speechThingy.setPoint(0, sf::Vector2f(rect.width * (1.f / 8.f), rect.height + 8));
            speechThingy.setPoint(1, sf::Vector2f(0, rect.height + 8 + gDistance * (1.f/3.f)));
            speechThingy.setPoint(2, sf::Vector2f(rect.width * (2.f/6.f), rect.height + 8));

            float alpha = 0.f;
            if (mTime > 4.f)
                alpha = (mTime-4);

            speechThingy.setFillColor(sf::Color(255,255,255,255-(255*alpha)));
            box.setFillColor(sf::Color(255, 255, 255, 255 - (255*alpha)));
            message.setColor(sf::Color(0, 0, 0, 255 - (255*alpha)));

            message.setOrigin(-8, rect.height / 2.f + gDistance);
            box.setOrigin(0, rect.height / 2.f + 8 + gDistance);
            speechThingy.setOrigin(0, rect.height / 2.f + gDistance);

            speechThingy.setPosition(mPosition);
            message.setPosition(mPosition);
            box.setPosition(mPosition);

            target.draw(box);
            target.draw(message);
            target.draw(speechThingy);
        });
    }
}