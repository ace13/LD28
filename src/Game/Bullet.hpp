#pragma once

#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf { class Texture; }

class Bullet : public Kunlaboro::Component
{
public:
    Bullet();
    ~Bullet();

    void addedToEntity();

    inline float getDamage() const { return mDamage; }

private:
    sf::Vector2f mPosition;
    sf::Texture* mTexture;
    float mDamage, mAngle, mLifeTime;
};
