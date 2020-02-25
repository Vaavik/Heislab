#ifndef LIB_H
#define LIB_H
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"
#include "floor.h"
#include "init.h"
#include "elevator.h"
#endif

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}


int main(){
    init();
    signal(SIGINT, sigint_handler);
    elevator();
    return 0;
}
