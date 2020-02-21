#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"
#include "elevator.h"


int idle(){
    int state;
    while(1){
        set_orders();
        if(order_inside[current_floor]||order_up[current_floor]){
            current_mission = current_floor;
            state = stop_up_state;
            goto end;
        }
        for(int f = 0; f < current_floor; f++){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_mission = f;
                state = moving_down_state;
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                goto end;
            }
        }
        for(int f = current_floor + 1; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_mission = f;
                state = moving_up_state;
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
                goto end;
            }
        }
    }
    end: ;
    return state;
}

void moving_up(){

}

void moving_down(){

}

void stop_up(){

}

void stop_down(){

}



void elevator(){ 
    int state = idle_state;
    while(1){
        switch (state) {
            case idle_state: {
                state = idle();
                break;
            }
            case moving_up_state: {
                moving_up();
                break;
            }
            case moving_down_state: {
                moving_down();
                break;
            }
            case stop_up_state: {
                stop_up();
                break;
            }
            case stop_down_state: {
                stop_down();
                break;
            }
        }

    }
}