// agent.h
#ifndef AGENT_H
#define AGENT_H

class Agent
{
    private:
        // A set of enumerated directions for use later
        enum direction { DOWN = 1, UP = -1, LEFT = -1, RIGHT = 1};

        // The size of the world
        const int WORLD_SIZE = 64;   

        // The set of discrete actions available for the agent to take
        int actions[4][2] = { { LEFT, 0 }, { 0, UP }, { RIGHT, 0 }, { 0, DOWN } };

        // The current position of the agent
        int position[2] = { 0, 0 };

        // The starting health of the agent
        int health = 100;

        // The starting bank balance of the agent
        int loot = 0;

    public:


        // Getters and setters for various private variables
        int* getPosition();
        void setPosition( int x, int y );
        int getHealth();
        int getLoot();

        /************************************
         * Functions to partially implement *
         ************************************/

        // Agent constructor
        // You need to implement a random start position for the agent in this constructor. 
        // This position must be within the world bounds
        Agent();

        /***********************************
         * Functions to implement entirely *
         ***********************************/

        // eat() should be called when the agent encounters a FOOD as defined in the world class
        // The result should be an increase in health by 5.
        void eat( );

        // mine() should be called when the agent encounters a GOLD as defined in the world class
        // The result should be an increase in loot by 1.
        void mine( );

        // takeRandomAction() should be called when the agent wants to make a random action
        // There are four random actions that can be taken as defined in the actions array above. 
        // This function should generate a random number and select the action from the array
        // After selecting the action, the position should be updated accordingly. 
        // If the agent would move out of bounds of the world, select another random action.
        void takeRandomAction();

        // takePlannedAction should be called when the agent wants to make a planned action
        // This can be implemented however you want. You should ensure that the agent does not go out of bounds as above. 
        // The strategy to move can be anything you think will optimize the result. 
        void takePlannedAction(); 

};

#endif