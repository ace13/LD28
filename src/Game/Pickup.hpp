#pragma once

#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf { class Texture; }

class Pickup : public Kunlaboro::Component
{
public:
    Pickup();
    ~Pickup();

    void addedToEntity();

    inline void setPosition(const sf::Vector2f& pos) { mPosition = pos; }

private:
    sf::Vector2f mPosition;
    sf::Texture* mTexture;
    float mAngle, mStartAngle, mInertia, mLifeTime;
};
