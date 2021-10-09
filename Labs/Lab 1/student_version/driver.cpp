#include "agent.h"
#include "world.h"
#include <iostream>

int main() {
    Agent* the_agent = new Agent( );
    World* the_world = new World( the_agent );  

    std::cout << "Simulation starting.\n";

    the_world->createWorld();
    the_world->run( World::RANDOM );

    the_world->createWorld();
    the_world->run( World::PLANNED );

    std::cout << "Simulation complete.\n";

    return 0;
}