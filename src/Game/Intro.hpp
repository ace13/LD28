#pragma once

#include <Kunlaboro/Component.hpp>
#include <SFML/Graphics/Texture.hpp>

class Intro : public Kunlaboro::Component
{
public:
    Intro();
    ~Intro();

    void addedToEntity();

private:
    float mIntroTime, mLastScale;
    sf::Texture mTexture;
};