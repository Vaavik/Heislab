#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"
#include "init.h"
#include "elevator.h"


static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void init(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    Orders orders = {{0,0,0,0},{0,0,0,0},{0,0,0,0},0};
    Orders * p_orders = &orders;

    clear_all_orders(p_orders);

    hardware_command_movement(HARDWARE_MOVEMENT_DOWN); //går ned til første etasje og setter etasjen.
    while(!hardware_read_floor_sensor(0)){}
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    printf("initializing complete\n");



}
