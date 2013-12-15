#pragma once

#include <SFML/System/Vector2.hpp>
#include <Kunlaboro/Component.hpp>
#include <map>
#include <list>
#include <functional>

namespace sf { class RenderTarget; }

class Menu : public Kunlaboro::Component
{
public:
    Menu();
    ~Menu();

    void addedToEntity();

    void drawUi(sf::RenderTarget& target, float dt);

private:
    std::list<std::pair<std::string, std::function<void()> > > mEntries;
    sf::Vector2f mMousePos, mScreenCenter;
    float mTime;

    bool mInGame;
};
