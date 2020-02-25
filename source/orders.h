/**
 * @file
 * @brief Organizes and handles the orders, and endstation.
 */


typedef struct{
    bool inside[HARDWARE_NUMBER_OF_FLOORS];
    bool up[HARDWARE_NUMBER_OF_FLOORS]; //kunne vært størrelse floors-1 men gjort slik pga enklere indeksering
    bool down[HARDWARE_NUMBER_OF_FLOORS];
    int endstation;
}Orders;


/**
 * @brief Checks if there are any new orders and adds them to @p p_orders.
 * Also updates the endstation @p p_orders->endstation to the last floor with an order in direction @p direction.
 * Turns on the respective button lights as well.
 * 
 * @param p_orders A pointer to the order struct that is being updated.
 * @param direction The direction the elevator is traveling (1 for up, 0 for down).
 */
void update_orders(Orders * p_orders, bool direction);

/**
 * @brief Clears order from floor @p floor of type @p order_type. Turns off respective button lights as well.
 * 
 * @param p_orders A pointer to the orders struct where the spesific order is being deleted.
 * @param floor Floor of the button.
 * @param order_type This decides which button on the floor to clear. The types are HARDWARE_ORDER_UP or HARDWARE_ORDER_DOWN or HARDWARE_ORDER_INSIDE
 */
void clear_order(Orders * p_orders, int floor, HardwareOrder order_type);

/**
 * @brief Clears all orders from the orders struct @p p_orders is pointing to, and turns off all button lights.
 * 
 * @param p_orders A pointer to the orders struct that is being cleared.
 */
void clear_all_orders(Orders * p_orders);