#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "hardware.h"
#include "orders.h"
#include "elevator.h"



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
            case stop_state: {
                state = stop();
                break;
            }
        }

    }
}

int idle(){
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    while(1){
        update_orders(1);

        if(hardware_read_stop_signal()){
            state = stop_state;
            goto end;
        }

        if(order_inside[current_floor]||order_up[current_floor]){  //denna seksjonen e stygg, pls fiks
            if(hardware_read_floor_sensor(current_floor)){
                current_endstation = current_floor;
                state = stop_up_state;
            }
            else{
                if(current_direction){state = moving_down_state;}
                else {state = moving_up_state;}
            }
            goto end;
        }
        if(order_down[current_floor]){
            if(hardware_read_floor_sensor(current_floor)){
                current_endstation = current_floor;
                state = stop_down_state;
            }
            else{
                if(current_direction){state = moving_down_state;}
                else {state = moving_up_state;}
            }
            goto end;
        }                                                       //stygg til hit


        for(int f = 0; f < current_floor; f++){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_endstation = f;
                state = moving_down_state;
                goto end;
            }
        }
        for(int f = HARDWARE_NUMBER_OF_FLOORS -1; f > current_floor; f--){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_endstation = f;
                state = moving_up_state;
                goto end;
            }
        }
    }
    end: ;
    return state;
}

int moving_up(){ 
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    current_direction = 1;
    while(1){
        update_orders(1);
        if(hardware_read_stop_signal()){
            state = stop_state;
            break;
        }
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                current_floor = f;
                hardware_command_floor_indicator_on(f);
                if(order_up[f]||order_inside[f]||current_endstation == f){ 
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
    current_direction = 0; 
    while(1){
        update_orders(0);
        if(hardware_read_stop_signal()){
            state = stop_state;
            break;
        }
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                current_floor = f;
                hardware_command_floor_indicator_on(f);
                if(order_down[f]||order_inside[f]||current_endstation == f){
                    state = stop_down_state;
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

    hardware_command_door_open(1);  //åpner døra her
    int sec = 0, trigger = 3;       //timer 3 sek
    clock_t before = clock();
    while(sec < trigger){

        if(hardware_read_stop_signal()){
            state = stop_state;
            goto end;
        }


        update_orders(1);            //tar imot ordre samtidig
        if(hardware_read_obstruction_signal()){  //Resetter timeren når obstruction er på
            before = clock();
        }

        clock_t diff = clock() - before;
        sec = diff/ CLOCKS_PER_SEC;
    }
    hardware_command_door_open(0);  //lukker døra her


    if(current_endstation > current_floor){
        state = moving_up_state;
    }
    else{
        state = idle_state;
    }

    end: ;

    return state;
}


int stop_down(){
    int state;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);  
    HardwareOrder order_types[3] = {    //clearer alle ordrene til etasjen //lag en fuckings global variabel idiot
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };
    for(int i = 0; i < 3; i++){
        HardwareOrder order_type = order_types[i];
        clear_order(current_floor, order_type);
    }
    
    hardware_command_door_open(1);  //åpner døra her
    int sec = 0, trigger = 3;       //timer 3 sek
    clock_t before = clock();
    while(sec < trigger){
        update_orders(0);            //tar imot ordre samtidig
        if(hardware_read_obstruction_signal()){  //Resetter timeren når obstruction er på
            before = clock();
        }

        clock_t diff = clock() - before;
        sec = diff/ CLOCKS_PER_SEC;
    }
    hardware_command_door_open(0);  //lukker døra her


    if(current_endstation < current_floor){
        state = moving_down_state;
    }
    else{
        state = idle_state;
    }

    return state;
}

int stop(){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);
    clear_all_orders();
    if(hardware_read_floor_sensor(current_floor)){
        hardware_command_door_open(1);
        while(hardware_read_stop_signal()){}
        hardware_command_stop_light(0);
        return stop_down_state;
    }
    else{
        while(hardware_read_stop_signal()){}
        hardware_command_stop_light(0);
        return idle_state;
    }
}