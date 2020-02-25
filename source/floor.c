/*
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include "hardware.h"
#include "floor.h"
/*
bool floor_update(Floor * floor){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_floor_sensor(f)){
            floor->last = floor->current;
            floor->current = f;
            hardware_command_floor_indicator_on(f);
            return 1;
        }
    }
    return 0;
}