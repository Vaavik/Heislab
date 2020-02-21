#include <stdbool.h>

int current_floor;
int current_mission;

bool order_inside[HARDWARE_NUMBER_OF_FLOORS];
bool order_up[HARDWARE_NUMBER_OF_FLOORS]; //kunne vært størrelse floors-1 men pga enklere kode gidd ikke
bool order_down[HARDWARE_NUMBER_OF_FLOORS];
//void set_current_mission();

void set_orders();

void clear_order(int floor, HardwareOrder order_type);

void clear_all_orders();