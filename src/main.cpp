#include "Engine.hpp"
#include <Kunlaboro/EntitySystem.hpp>

int main(int argc, char** argv)
{
    Kunlaboro::EntitySystem sys;
    Engine eng(sys);

    return eng.mainLoop();
}
