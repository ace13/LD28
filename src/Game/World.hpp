#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>
#include <SFML/Graphics/Texture.hpp>

struct Stats
{
    Stats():ShotsFired(0),WeaponsThrown(0),ShotsHit(0),EnemiesDead(0),EnemiesDeadByPlayer(0), DistanceTraveled(0),MaxSpeed(0) {}

    int ShotsFired, WeaponsThrown, ShotsHit, EnemiesDead, EnemiesDeadByPlayer;
    float DistanceTraveled, MaxSpeed;

};

class World : public Kunlaboro::Component
{
public:
    World();
    ~World();

    void addedToEntity();



private:
    SpriteSheet mSheet;
    sf::Texture mBigOne;
    Stats mStats;
};
