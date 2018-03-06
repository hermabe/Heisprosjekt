#include "buttonRead.h"

/******************************************************
 * 
 * 
 * 
 * ****************************************************/
void read_buttons_and_light_up_button() {
    for (int floor=0; floor<N_FLOORS;floor++) {
        for (elev_button_type_t button = BUTTON_CALL_UP; button < 3; button++) {
            if (elev_get_button_signal(button, floor)==1) {
                elev_set_button_lamp(button, floor, 1);
            }
        }
    }
}

