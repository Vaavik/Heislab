#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"
#include "elevator.h"


int idle(){
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    while(1){
        update_orders();
        if(order_inside[current_floor]||order_up[current_floor]){
            current_destination = current_floor;
            state = stop_up_state;
            goto end;
        }
        for(int f = 0; f < current_floor; f++){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_destination = f;
                state = moving_down_state;
                goto end;
            }
        }
        for(int f = HARDWARE_NUMBER_OF_FLOORS -1; f > current_floor; f--){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_destination = f;
                state = moving_up_state;
                goto end;
            }
        }
    }
    end: ;
    return state;
}

int moving_up(){ //har ikke implementert current_destination
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while(1){
        update_orders();
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                current_floor = f;
                hardware_command_floor_indicator_on(f);
                if(order_up[f]||order_inside[f]||current_destination == f){ 
                    state = stop_up_state;
                    goto end;
                }

            }
        }
    }
    end: ;
    return state;

}

int moving_down(){
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    while(1){
        update_orders();
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                current_floor = f;
                hardware_command_floor_indicator_on(f);
                if(order_down[f]||order_inside[f]||current_destination == f){
                    state = stop_up_state;
                    goto end;
                }

            }
        }
    }
    end: ;
    return state;

}

int stop_up(){
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };
    for(int i = 0; i < 3; i++){
        HardwareOrder order_type = order_types[i];
        clear_order(current_floor, order_type);
    }
    //fiks åpne dør her
    if(current_destination > current_floor){
        state = moving_up_state;
    }
    else{
        state = idle_state;
    }

    return state;
}

int stop_down(){
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };
    for(int i = 0; i < 3; i++){
        HardwareOrder order_type = order_types[i];
        clear_order(current_floor, order_type);
    }
    //fiks åpne dør her
    if(current_destination < current_floor){
        state = moving_down_state;
    }
    else{
        state = idle_state;
    }

    return state;
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
                state = moving_up();
                break;
            }
            case moving_down_state: {
                state = moving_down();
                break;
            }
            case stop_up_state: {
                state = stop_up();
                break;
            }
            case stop_down_state: {
                state = stop_down();
                break;
            }
        }
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        }

    }
}