#include "elev.h"
#include <stdio.h>
#include "controller.h"

int main() {
    printf("Press STOP button to stop elevator and exit program.\n");

    startup();
    
    elev_set_motor_direction(DIRN_UP);

     while (1) {
         // Stop elevator and exit program if the stop button is pressed
         if (elev_get_stop_signal()) {
             elev_set_motor_direction(DIRN_STOP);
             break;
         }
     }

    return 0;
}
