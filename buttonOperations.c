#include "buttonOperations.h"

void read_buttons_and_light_up_buttons() {
    for (int floor=0; floor<N_FLOORS;floor++) {
        for (elev_button_type_t button = BUTTON_CALL_UP; button < 3; button++) {
            if (elev_get_button_signal(button, floor)==1) {
                elev_set_button_lamp(button, floor, 1);
            }
        }
    }
}

void reset_button_lights_at_floor(int floor) {
    for (elev_button_type_t button = 0; button < 3; ++button)
    {
        elev_set_button_lamp(button, floor, 0);
    }
}

void reset_all_lights_except_stop_light() {
    for (int floor = 0; floor < 4; floor++)
    {
        reset_button_lights_at_floor(floor);
    }
}
