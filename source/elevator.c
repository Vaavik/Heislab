#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "hardware.h"
#include "orders.h"
#include "elevator.h"

#define up 1
#define down 0


void elevator(){ 
    int state = idle_state;
    while(1){
        switch (state) {
            case idle_state: {
                state = idle();
                break;
            }
            case moving_up_state: {
                state = moving(up);
                break;
            }
            case moving_down_state: {
                state = moving(down);
                break;
            }
            case stop_up_state: {
                state = stop(up);
                break;
            }
            case stop_down_state: {
                state = stop(down);
                break;
            }
            case stop_state: {
                state = emergency_stop();
                break;
            }
        }

    }
}

int idle(){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    while(1){
        update_orders(1);

        if(hardware_read_stop_signal()){return stop_state;}

        if(order_inside[current_floor]||order_up[current_floor]){  //denna seksjonen e stygg, pls fiks, den e her for å fikse det som skjer hvis man trykke på samme etasje
            if(hardware_read_floor_sensor(current_floor)){
                current_endstation = current_floor; //denne er unødvendig tror jeg, siden den allerede er på current_endstation. test å ta det bort senere. fuck emergency stop
                return stop_up_state;
            }
            else if(current_direction){return moving_down_state;}  //bare sett en av de som deafult
                else {return moving_up_state;}
        }
        if(order_down[current_floor]){
            if(hardware_read_floor_sensor(current_floor)){ //trenger ikke denne ifen av samme grunn som else foran. ops den trengs pga emergency stop
                current_endstation = current_floor;  // trenger ikke, tror den trengs pga emergency stop
                return stop_down_state;
            }
            else{ //denne else trenger ikke å være her, fordi den ikke kan bevege seg da vi fjerner etterpå. eller jo! den trengs pra emergency stop
                if(current_direction){return moving_down_state;}
                else {return moving_up_state;}
            }
        }                                                       //stygg til hit


        for(int f = 0; f < current_floor; f++){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_endstation = f;
                return moving_down_state;
            }
        }
        for(int f = HARDWARE_NUMBER_OF_FLOORS -1; f > current_floor; f--){
            if(order_inside[f] || order_down[f] || order_up[f]){
                current_endstation = f;
                return moving_up_state;
            }
        }
    }

}


int moving(bool direction){

    if(direction){hardware_command_movement(HARDWARE_MOVEMENT_UP);}
    else{hardware_command_movement(HARDWARE_MOVEMENT_DOWN);}
    current_direction = direction;
    while(1){
        update_orders(direction);
        if(hardware_read_stop_signal()){return stop_state;}

        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                current_floor = f;
                hardware_command_floor_indicator_on(f);
                if(direction && (order_up[f]||order_inside[f]||current_endstation == f)){return stop_up_state;}
                if(!direction && (order_down[f]||order_inside[f]||current_endstation == f)){return stop_down_state;}
            }
        }
    }
    

}



int stop(bool direction){
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

        if(hardware_read_stop_signal()){return stop_state;}


        update_orders(1);            //tar imot ordre samtidig
        if(hardware_read_obstruction_signal()){  //Resetter timeren når obstruction er på
            before = clock();
        }

        clock_t diff = clock() - before;
        sec = diff/ CLOCKS_PER_SEC;
    }
    hardware_command_door_open(0);  //lukker døra her

    if(direction && (current_endstation > current_floor)){return moving_up_state;}          //dont touch
    else if(direction && (current_endstation < current_floor)){return moving_down_state;}
    else{return idle_state;}
}


int emergency_stop(){
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