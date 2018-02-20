#include "controller.h"


void startup() {
    printf("Initializing elevator\n");
     // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        exit(1);
    }

    // Put elevator in 1st floor
    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal() != (int)0)
    {
        if (elev_get_stop_signal())
        {
            elev_set_motor_direction(DIRN_STOP);
            printf("Startup aborted\n");
            exit(0);
        }
    }
    elev_set_motor_direction(DIRN_UP);
    elev_set_motor_direction(DIRN_STOP);
    printf("Initialization done\n");
}

bool is_specific_queue_empty(const bool queue[], const int size){
    for(int i = 0; i < size; ++i){
        if (queue[i]){
            return false;
        }
    }
    return true;
}

bool is_queue_empty(elev_motor_direction_t dir, const Controller ctrl){
    if (dir == DIRN_DOWN){
        return is_specific_queue_empty(ctrl.down_queue, N_FLOORS);
    }
    else if (dir == DIRN_UP){
        return is_specific_queue_empty(ctrl.up_queue, N_FLOORS);
    }
    else {
        return is_specific_queue_empty(ctrl.down_queue, N_FLOORS) && is_specific_queue_empty(ctrl.up_queue, N_FLOORS);
    }
}

elev_motor_direction_t up_or_down_from_idle(const Controller ctrl){
    bool is_up_empty = is_queue_empty(DIRN_UP, ctrl);
    bool is_down_empty = is_queue_empty(DIRN_DOWN, ctrl);
    if (is_down_empty && is_up_empty){
        return DIRN_STOP;
    }
    assert(ctrl.current_floor <= 4 && ctrl.current_floor >= 1);
    switch (ctrl.current_floor){
        case 1:
            return DIRN_UP;
        case 4:
            return DIRN_DOWN;
        default:
            if (!is_up_empty){
                return DIRN_DOWN;
            }
            else {
                return DIRN_UP;
            }
    }
}