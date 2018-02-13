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

bool is_queue_empty(const bool queue[], const int size){
    for(int i = 0; i < size; ++i){
        if (queue[i]){
            return false;
        }
    }
    return true;
}

bool is_queue_empty(elev_motor_direction_t dir, const Controller ctrl){
    assert(dir != DIRN_STOP);
    if (dir == DIRN_DOWN){
        return is_queue_empty(ctrl.down_queue, ctrl.number_of_floors);
    }
    else if (dir == DIRN_UP){
        return is_queue_empty(ctrl.up_queue, ctrl.number_of_floors);
    }

}
