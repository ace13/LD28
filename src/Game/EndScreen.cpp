#include "EndScreen.hpp"
#include "World.hpp"
#include "../Resources.hpp"
#include <tuple>

EndScreen::EndScreen() : Kunlaboro::Component("Game.EndScreen")
{
}

EndScreen::~EndScreen()
{
}

void EndScreen::addedToEntity()
{
    auto reply = sendGlobalQuestion("Did I accidentally all the stats?");
    Stats stat;
    if (reply.handled)
        stat = boost::any_cast<Stats>(reply.payload);

    sendGlobalMessage("I'm ending this!");

    requestMessage("Event.DrawUi", [this, stat](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload);
        auto& target = *std::get<0>(data);

        sf::Text daEnd("DE_END", Resources::Font_Dosis, 64);
        daEnd.setOrigin(daEnd.getLocalBounds().width / 2.f, daEnd.getLocalBounds().height / 2.f);
        daEnd.setPosition(target.getView().getSize().x / 2.f, 48);       
        target.draw(daEnd);

        daEnd.setCharacterSize(36);
        daEnd.setString("During this round");
        daEnd.setOrigin(0, daEnd.getLocalBounds().height / 2.f);
        daEnd.setPosition(10, 168);
        target.draw(daEnd);

        daEnd.setCharacterSize(30);

        char tmp[256];
        sprintf(tmp, "%i", stat.EnemiesDeadByPlayer);

        daEnd.move(0, daEnd.getLocalBounds().height + 32);
        daEnd.setString(std::string(tmp) + " enemies were brutally murdered by the player.");
        target.draw(daEnd);

        sprintf(tmp, "%i", stat.EnemiesDead);

        daEnd.move(0, daEnd.getLocalBounds().height + 16);
        daEnd.setString(std::string(tmp) + " were betrayed by those they considered friends.");
        target.draw(daEnd);

        sprintf(tmp, "%i", stat.ShotsFired);

        daEnd.move(0, daEnd.getLocalBounds().height + 16);
        daEnd.setString(std::string(tmp) + " shots were heard.");
        target.draw(daEnd);

        sprintf(tmp, "%.2f%%", (float)stat.ShotsHit / (float)stat.ShotsFired);

        daEnd.move(0, daEnd.getLocalBounds().height + 16);
        daEnd.setString(std::string(tmp) + " of the bullets actually hit.");
        target.draw(daEnd);

        sprintf(tmp, "%.2f", stat.DistanceTraveled);

        daEnd.move(0, daEnd.getLocalBounds().height + 16);
        daEnd.setString(std::string(tmp) + " meters of terrain was travelled across.");
        target.draw(daEnd);

        sprintf(tmp, "%.2f km/h", stat.MaxSpeed);

        daEnd.move(0, daEnd.getLocalBounds().height + 16);
        daEnd.setString("And the player strolled casually along at " + std::string(tmp) + "...");
        target.draw(daEnd);
    });
}