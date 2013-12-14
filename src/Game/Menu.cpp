#include "Menu.hpp"
#include "../Resources.hpp"

#include <Kunlaboro/EntitySystem.hpp>

#include <tuple>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

Menu::Menu() : Kunlaboro::Component("Game.Menu"), mTime(0), mInGame(false)
{
    
}

Menu::~Menu()
{
}

void Menu::addedToEntity()
{
    ///\TODO Put initialization somewhere else?
    {
        auto& sys = *getEntitySystem();
        Kunlaboro::Message question(Kunlaboro::Type_Message, this);
        sys.sendGlobalMessage(sys.getMessageRequestId(Kunlaboro::Reason_Message, "Is the game running?"), question);

        if (question.handled && boost::any_cast<bool>(question.payload))
        {
            mInGame = true;

            mEntries.push_back(std::make_pair("Continue Game", [this, &sys]()
            {
                sys.destroyEntity(getOwnerId());
            }));
            mEntries.push_back(std::make_pair("End Game", [this]() { sendGlobalMessage("ExitGame"); }));
        }
        else
        {
            mInGame = false;

            mEntries.push_back(std::make_pair("Start Game", [this]()
            {
                auto& sys = *getEntitySystem();
        
                auto ent = sys.createEntity();
                sys.addComponent(ent, "Game.Player");
                sys.addComponent(ent, "Game.Weapon");

                ent = sys.createEntity();
                sys.addComponent(ent, "Game.Enemy");
                sys.addComponent(ent, "Game.Weapon");

                sys.destroyEntity(getOwnerId());
            }));
            mEntries.push_back(std::make_pair("End Game", [this]() { sendGlobalMessage("ExitGame"); }));
        }
    }

    requestMessage("Is the game paused?", [](Kunlaboro::Message& msg) { msg.handled = true; msg.payload = true; });
    requestMessage("Event.Update", [this](Kunlaboro::Message& msg)
    {
        update(boost::any_cast<float>(msg.payload));

        if (mInGame)
            msg.handled = true;
    });
    changeRequestPriority("Event.Update", -1);
    requestMessage("Event.DrawUi", [this](const Kunlaboro::Message& msg)
    {
        drawUi(*boost::any_cast<sf::RenderTarget*>(msg.payload));
    });
    changeRequestPriority("Event.DrawUi", 1);
    requestMessage("Event.Mouse.Click", [this](Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::Mouse::Button, sf::Vector2f, bool> >(msg.payload); 

        sf::Text string("MENU ENTRY GO HERE", Resources::Font_Dosis);

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
        
        msg.handled = true;
    });
    changeRequestPriority("Event.Mouse.Click", -1);
    requestMessage("Event.Mouse.Move", [this](Kunlaboro::Message& msg)
    {
        msg.handled = true;

        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        mMousePos = pos;
    });
    changeRequestPriority("Event.Mouse.Move", -1);

    if (mInGame)
    {
        requestMessage("Event.Mouse.MoveGame", [](Kunlaboro::Message& msg) { msg.handled = true; });
        changeRequestPriority("Event.Mouse.MoveGame", -1);
    }
}

void Menu::update(float dt)
{
    mTime += dt;
}

void Menu::drawUi(sf::RenderTarget& target)
{
    if (mInGame)
    {
        sf::RectangleShape shape(target.getView().getSize());
        shape.setFillColor(sf::Color(0,0,0, std::min(mTime, 0.75f)*255));
        target.draw(shape);
    }

    sf::Text string(Resources::String_Name, Resources::Font_Dosis);

    mScreenCenter = target.getView().getCenter();

    string.setCharacterSize(120);
    string.setPosition(mScreenCenter.x, 60);

    string.rotate(sin(mTime) * 3.1415);

    auto rect = string.getLocalBounds();
    string.setOrigin(rect.width / 2.f, rect.height / 2.f);

    target.draw(string);

    string.setCharacterSize(30);
    string.setRotation(0);
    string.setPosition(mScreenCenter);

    for (auto it = mEntries.begin(); it != mEntries.end(); ++it)
    {
        auto& tuple = *it;
        
        string.setString(tuple.first);
        rect = string.getLocalBounds();
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