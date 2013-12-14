#include "SpriteSheet.hpp"
#include <SFML/Graphics/Texture.hpp>

SpriteSheet::SpriteSheet(sf::Texture& texture, unsigned char width, unsigned char height) : mTexture(texture)
{
    auto tSize = texture.getSize();
    mSize = sf::Vector2u(width, height);
    mSpriteSize = sf::Vector2u((float)tSize.x / mSize.x, (float)tSize.y / mSize.y);


}

SpriteSheet::~SpriteSheet()
{
}

sf::IntRect SpriteSheet::getRect(unsigned char x, unsigned char y)
{
    return sf::IntRect(mSpriteSize.x * x, mSpriteSize.y * y, mSpriteSize.x, mSpriteSize.y);
}
