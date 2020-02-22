/**
 * @file
 * @brief Organizes and handles the orders, and current and next destination.
 */

#include <stdbool.h>

int current_floor;
int current_destination; 

bool order_inside[HARDWARE_NUMBER_OF_FLOORS];
bool order_up[HARDWARE_NUMBER_OF_FLOORS]; //kunne vært størrelse floors-1 men pga enklere kode gidd ikke
bool order_down[HARDWARE_NUMBER_OF_FLOORS];
//void set_current_destination();



/**
 * @brief Checks if there are any new orders and adds them to @c order_inside , @c order_up and @c order_down . Turns on the respective button lights as well.
 * 
 */
void update_orders(bool direction);

/**
 * @brief Clears order from floor @p floor of type @p order_type. Turns off respective button lights as well.
 * 
 * @param floor floor
 * @param order_type HARDWARE_ORDER_UP or HARDWARE_ORDER_DOWN or HARDWARE_ORDER_INSIDE
 */
void clear_order(int floor, HardwareOrder order_type);

/**
 * @brief Clears all orders and turns off all button lights.
 */
void clear_all_orders();