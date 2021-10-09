// world.h
#ifndef WORLD_H
#define WORLD_H

#include "agent.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <assert.h>     /* assert */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

class World
{
    private:
        // Values corresponding to the items in the world
        enum items { NOTHING, FOOD, GOLD };

        // The agent who is exploring this world
        Agent* agent;

        // The size of the world
        static const int WORLD_SIZE = 64;  

        // The array that contains the elements of the world
        int world[WORLD_SIZE][WORLD_SIZE];

    public:
        // Types of searching for the driver to be more readable
        enum search_type { RANDOM, PLANNED };

        // World constructor
        World( Agent* a );

        // checkAgent throws an error if the agent goes out of bounds
        void checkAgent( );

        // This function is used by the driver to explore the world
        void run(int search_type);

        /************************************
         * Functions to partially implement *
         ************************************/
        // The log function will print the current state of the world. This will take the form of a checkered square where each box corresponds to a cell in the world array.
        // You must assign the integer values between 0 and 255 to form the following colors (RGB based) based on the material:
        // GOLD: Yellow
        // FOOD: Green
        // NOTHING: Black
        // Agent Position: Blue
        //
        // Additionally you must specify the correct rect_size, where rect_size is how many pixels on the resulting image each grid cell corresponds to. This is used to draw the resulting image
        void log(int step);

        /***********************************
         * Functions to implement entirely *
         ***********************************/

        // createWorld function creates the world for the agent to live in
        // populate the world class variable with random values (0, 1, 2), These correspond to the items enumerated values above
        void createWorld();

        // take_random_step tells the agent to take a random step. 
        // The checkAgent function must not be called to verify if the agent is in bounds
        // If valid the resulting position is then checked against the world to see if the agent needs to eat or loot the square. 
        // The required action is then performed, and the value in the world is set to 0 as it no longer contains anything
        void take_random_step(); 

        // take_planned_step tells the agent to take a planned step. 
        // The checkAgent function must not be called to verify if the agent is in bounds
        // If valid the resulting position is then checked against the world to see if the agent needs to eat or loot the square. 
        // The required action is then performed, and the value in the world is set to 0 as it no longer contains anything
        void take_planned_step(); 
};

#endif