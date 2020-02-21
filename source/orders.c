#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"


void set_orders(){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            /* Internal orders */
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
                order_inside[f] = 1;
            }

            /* Orders going up */
            if(hardware_read_order(f, HARDWARE_ORDER_UP)){
                hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
                order_up[f] = 1;
            }

            /* Orders going down */
            if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
                hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
                order_down[f] = 1;

            }
        }
}

void clear_order(int floor, HardwareOrder order_type){
    hardware_command_order_light(floor, order_type, 0);
    if(order_type == HARDWARE_ORDER_INSIDE){
                order_inside[floor] = 0;
    }
    if(order_type == HARDWARE_ORDER_UP){
                order_up[floor] = 0;
    }
    if(order_type == HARDWARE_ORDER_DOWN){
                order_down[floor] = 0;
    }

}

void clear_all_orders(){
        HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder order_type = order_types[i];
            clear_order(f, order_type);
        }
    }
}