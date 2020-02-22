#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"
#include "init.h"
#include "elevator.h"




int main(){
    init();


    printf("=== Example Program ===\n");
    
    elevator();

    printf("Press the stop button on the elevator panel to exit\n");


    hardware_command_movement(HARDWARE_MOVEMENT_UP);

    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        }

        if(hardware_read_floor_sensor(0)){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }

        /* All buttons must be polled, like this: */
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_floor_indicator_on(f);
            }
        }


        update_orders(1);
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                clear_order(f, HARDWARE_ORDER_INSIDE);
                clear_order(f, HARDWARE_ORDER_UP);
                clear_order(f, HARDWARE_ORDER_DOWN);
            }
        }
        



        if(hardware_read_obstruction_signal()){
            hardware_command_stop_light(1);
        }
        else{
            hardware_command_stop_light(0);
        }

        
    }

    return 0;
}
