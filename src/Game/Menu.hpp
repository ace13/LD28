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

    void update(float dt);
    void drawUi(sf::RenderTarget& target);

private:
    std::list<std::pair<std::string, std::function<void()> > > mEntries;
    sf::Vector2f mMousePos, mScreenCenter;
};
