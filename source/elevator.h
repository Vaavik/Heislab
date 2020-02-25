/**
 * @file
 * @brief The state machine, and functions for each state.
 */


#define idle_state 0
#define moving_up_state 1
#define moving_down_state 2
#define stop_up_state 3
#define stop_down_state 4
#define stop_state 5

/**
 * @brief The state machine, switches between states.
 */

void elevator();

/**
 * @brief The idle state, updates the orders until a new state is set. When there is a new order, it will change the state according to the first new order. 
 * Order in floor above or below will set the state to @p moving_up_state or @p moving_down_state respectively.
 * Order in the same floor will set the state to @p stop_up_state or @p stop_down_state, depending on the button direction (up/down, panel_button sets it in @p stop_up_state).
 * If the stop button is pressed, it will set the state to @p stop_state.
 * @return The new state.
 */

int idle();

/**
 * @brief The moving up state. Sets the motor to @c HARDWARE_MOVEMENT_UP. 
 * Continously updates orders and checks if it is on a floor with an order of type @c order_up, @c order_inside, or if the endpoint @c current_endpoint is reached.
 * If any of the above conditions are met, the state is set to @c stop_up.
 * If the stop button is pressed, the state is set to @c stop.
 * After the state is changed the function returns.
 *  @return The new state. 
 */

int moving(bool direction);


/**
 * @brief The stop up state. Sets the motor to @c HARDWARE_MOVEMENT_STOP.
 * Continously updates orders, while opening the door for three seconds before closing it again. 
 * After the door is closed, it will set the state to state @c idle_state if the elevator is on floor @c current_endstation, or to stop_up_state if not.
 * If the stop button is pressed, the state is set to @c stop.
 * After the state is changed the function returns.
 * @return The new state.
 */

int stop(bool direction);
/**
 * @brief The stop state. Sets the motor to @c HARDWARE_MOVEMENT_STOP, clears all orders and turns on the stop light for as long  
 * 
 *
 */

int emergency_stop();