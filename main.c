#include "elev.h"
#include <stdio.h>

void initialize(){
    printf("Initializing elevator\n");
    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal() != (int)0) {}
    elev_set_motor_direction(DIRN_UP);
    elev_set_motor_direction(DIRN_STOP);
}


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    initialize();
    printf("Idle");
    
    // while (1) {
    //     // Change direction when we reach top/bottom floor
    //     if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
    //         elev_set_motor_direction(DIRN_DOWN);
    //     } else if (elev_get_floor_sensor_signal() == 0) {
    //         elev_set_motor_direction(DIRN_UP);
    //     }

    //     // Stop elevator and exit program if the stop button is pressed
    //     if (elev_get_stop_signal()) {
    //         elev_set_motor_direction(DIRN_STOP);
    //         break;
    //     }
    // }

    return 0;
}
