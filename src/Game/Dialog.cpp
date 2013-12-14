#include "Dialog.hpp"
#include "../Resources.hpp"
#include <Kunlaboro/EntitySystem.hpp>

const float gDistance = 96;

Dialog::Dialog(): Kunlaboro::Component("Game.Dialog"), mTime(0)
{
}

Dialog::~Dialog()
{

}

void Dialog::addedToEntity()
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
        auto& target = *boost::any_cast<sf::RenderTarget*>(msg.payload);

        sf::Text message(mMessage, Resources::Font_Dosis);
        auto rect = message.getLocalBounds();
        sf::RectangleShape box(sf::Vector2f(rect.width + 16, rect.height + 16));

        float alpha = 0.f;
        if (mTime > 4.f)
            alpha = (mTime-4);

        box.setFillColor(sf::Color(255, 255, 255, 255 - (255*alpha)));
        message.setColor(sf::Color(0, 0, 0, 255 - (255*alpha)));

        message.setOrigin(rect.width / 2.f, rect.height / 2.f + gDistance);
        box.setOrigin(rect.width / 2.f + 8, rect.height / 2.f + 8 + gDistance);
        message.setPosition(mPosition);
        box.setPosition(mPosition);

        target.draw(box);
        target.draw(message);
    });
}