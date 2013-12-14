#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf { class Texture; }

class SpriteSheet
{
public:
    SpriteSheet(sf::Texture& texture, unsigned char width, unsigned char height);
    ~SpriteSheet();

    sf::IntRect getRect(unsigned char x, unsigned char y);

private:
    sf::Texture& mTexture;
    sf::Vector2u mSize;
    sf::Vector2u mSpriteSize;
};
