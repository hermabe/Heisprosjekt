#include "elev.h"
#include <stdio.h>
<<<<<<< HEAD
#include "controller.h"
=======
#include "buttonRead.h"
>>>>>>> buttonread

int main() {
    printf("Press STOP button to stop elevator and exit program.\n");

<<<<<<< HEAD
    startup();
    
    elev_set_motor_direction(DIRN_UP);

     while (1) {
         // Stop elevator and exit program if the stop button is pressed
         if (elev_get_stop_signal()) {
             elev_set_motor_direction(DIRN_STOP);
             break;
         }
     }
=======

    elev_set_motor_direction(DIRN_UP);

    while (1) {
        // Change direction when we reach top/bottom floor
        
        
        
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        //Light ups buttons that are pressed
        read_buttons_and_light_up_button();
        
        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }
>>>>>>> buttonread

    return 0;
}
