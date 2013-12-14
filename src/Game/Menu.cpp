#include "Menu.hpp"
#include "../Resources.hpp"

#include <tuple>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

Menu::Menu() : Kunlaboro::Component("Game.Menu")
{
    mEntries.push_back(std::make_pair("Start Game", [this]() { }));
    mEntries.push_back(std::make_pair("End Game", [this]() { sendGlobalMessage("ExitGame"); }));
}

Menu::~Menu()
{
}

void Menu::addedToEntity()
{
    requestMessage("Event.Update", [this](const Kunlaboro::Message& msg)
    {
        update(boost::any_cast<float>(msg.payload));
    });
    requestMessage("Event.DrawUi", [this](const Kunlaboro::Message& msg)
    {
        //drawUi(*boost::any_cast<sf::RenderTarget*>(msg.payload));
    });
    requestMessage("Event.Mouse.Click", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::Mouse::Button, sf::Vector2f, bool> >(msg.payload); 

        sf::Text string("MENU ENTRY GO HERE", *Resources::Font_Dosis);

        string.setPosition(mScreenCenter);

        for (auto it = mEntries.begin(); it != mEntries.end(); ++it)
        {
            auto& tuple = *it;
        
            string.setString(tuple.first);
            auto rect = string.getLocalBounds();
            string.setOrigin(rect.width / 2.f, rect.height / 2.f);
            rect = string.getGlobalBounds();

            bool pressed = rect.contains(mMousePos);

            string.move(0, rect.height + 16);

            if (!pressed)
                continue;

            tuple.second();
        }
    });
    requestMessage("Event.Mouse.Move", [this](const Kunlaboro::Message& msg)
    {
        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        mMousePos = pos;
    });
}

void Menu::update(float dt)
{

}

void Menu::drawUi(sf::RenderTarget& target)
{
    sf::Text string("MENU ENTRY GO HERE", *Resources::Font_Dosis);

    mScreenCenter = target.getView().getCenter();
    string.setPosition(mScreenCenter);

    for (auto it = mEntries.begin(); it != mEntries.end(); ++it)
    {
        auto& tuple = *it;
        
        string.setString(tuple.first);
        auto rect = string.getLocalBounds();
        string.setOrigin(rect.width / 2.f, rect.height / 2.f);
        rect = string.getGlobalBounds();

        bool hovered = rect.contains(mMousePos);

        if (hovered)
        {
            string.setColor(sf::Color(128, 196, 0));
        }
        else
            string.setColor(sf::Color::White);


        target.draw(string);

        string.move(0, rect.height + 16);
    }
}