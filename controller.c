#include "controller.h"


void startup()
{
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






































































bool remove_floor(elev_motor_direction_t direction, controller_t ctrl, floor) {
    assert(direction != DIRN_STOP);
    if (direction == DIRN_UP && up_queue[floor] == 1)  {
        up_queue[floor] = 0;
        return true;
    } else if (direction == DIRN_UP && down_queue[floor] == 1) {
        down_queue[floor] = 0;
        return true;
    } else {
        return false;
    }
}

void reached_a_floor(elev_motor_direction_t direction, controller_t ctrl) {
    int floor=(elev_get_floor_sensor_signal();
    if (floor!=-1) {
        if (((direction == DIRN_UP) || (direction == DIRN_DOWN)) && remove_floor()) {
            if (direction == DIRN_UP) {
                ctrl.state = UPWAIT;
            } else if (direction == DIRN_DOWN) {
                ctrl.state = DOWNWAIT;
            }
        }
    }
}
