#pragma once

#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

#include <list>
#include <functional>

namespace sf { class RenderTarget; class View; }

class InfoScreen : public Kunlaboro::Component
{
public:
    InfoScreen();
    ~InfoScreen();

    void addedToEntity();

private:
    std::list<std::function<void(sf::RenderTarget&)>> mPages;

    const sf::View* mView;
    sf::Vector2f mMousePos;
    float mTime;
    bool mFirstUpdate;
    int mPage;
};
