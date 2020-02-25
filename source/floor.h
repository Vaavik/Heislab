
/**
 * @file
 * @brief The state machine, and functions for each state.
 */


typedef struct{
    int current;
    bool above;
}Floor;

bool floor_update(Floor * floor, bool direction);


/**
 * @brief Updates the current floor and last floor. sets floorlights. If it is on a floor.
 * 
 * @return 1 if the floor was changed, 0 if not.
 */

bool floor_update(Floor * floor, bool direction);
