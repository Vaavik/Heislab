/**
 * @file
 * @brief Organizes and handles the orders, and endstation.
 */

#include <stdbool.h>

bool current_direction; //1 for up 0 for down

typedef struct{
    int current;
    int last;
}Floor;

bool floor_update(Floor * floor);


typedef struct{
    bool inside[HARDWARE_NUMBER_OF_FLOORS];
    bool up[HARDWARE_NUMBER_OF_FLOORS]; //kunne vært størrelse floors-1 men gjort slik pga enklere indeksering
    bool down[HARDWARE_NUMBER_OF_FLOORS];
    int endstation;
}Orders;


bool order_inside[HARDWARE_NUMBER_OF_FLOORS];
bool order_up[HARDWARE_NUMBER_OF_FLOORS]; //kunne vært størrelse floors-1 men gjort slik pga enklere indeksering
bool order_down[HARDWARE_NUMBER_OF_FLOORS];
//void set_current_endstation();

/**
 * @brief Checks if there are any new orders and adds them to @c order_inside , @c order_up and @c order_down.
 * Also updates @c current_endstation to the last floor with an order in direction @p direction.
 * Turns on the respective button lights as well.
 * 
 * @p direction The direction the elevator is traveling (1 for up, 0 for down).
 */
void update_orders(Orders * p_orders, bool direction);

/**
 * @brief Clears order from floor @p floor of type @p order_type. Turns off respective button lights as well.
 * 
 * @param floor Floor of the button.
 * @param order_type This decides which button on the floor to clear. The types are HARDWARE_ORDER_UP or HARDWARE_ORDER_DOWN or HARDWARE_ORDER_INSIDE
 */
void clear_order(Orders * p_orders, int floor, HardwareOrder order_type);

/**
 * @brief Clears all orders and turns off all button lights.
 */
void clear_all_orders(Orders * p_orders);