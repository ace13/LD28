#pragma once

/*

######   ###   ###    ###
  #     #   #  #  #  #   #
  #     #   #  #  #  #   #
  #     #   #  #  #  #   #
  #      ###   ###    ###

*/

#include <Kunlaboro/Component.hpp>

class Particle : public Kunlaboro::Component
{
public:
    Particle();
    ~Particle();

    void addedToEntity();

private:
    
};