#include "agent.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Agent::Agent()
{
    srand(time(NULL));

    // Implement a random start position, see .h file for directions
}

int* Agent::getPosition()
{
    return this->position;
}

void Agent::setPosition( int x, int y )
{
    this->position[0] = x;
    this->position[1] = y;
}

void Agent::takeRandomAction()
{
    // Implement me, see .h file for directions
}

void Agent::takePlannedAction()
{
    // Implement me, see .h file for directions
}

int Agent::getHealth()
{
    return this->health;
}

int Agent::getLoot()
{
    return this->loot;
}

void Agent::eat( )
{
    // Implement me, see .h file for directions
}

void Agent::mine( )
{
    // Implement me, see .h file for directions
}