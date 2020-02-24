#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "orders.h"
#include "init.h"
#include "elevator.h"




int main(){
    init();    
    elevator();
    return 0;
}
