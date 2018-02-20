#include "controller.h"
#include <assert.h>

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


bool remove_floor(elev_motor_direction_t direction, Controller_t ctrl, int floor) {
    assert(direction != DIRN_STOP);
    if (direction == DIRN_UP && ctrl.up_queue[floor] == 1)
    {
        ctrl.up_queue[floor] = 0;
        return true;
    }
    else if (direction == DIRN_UP && ctrl.down_queue[floor] == 1)
    {
        ctrl.down_queue[floor] = 0;
        return true;
    } else {
        return false;
    }
}

void reached_a_floor(elev_motor_direction_t direction, Controller_t ctrl) {
    int floor = elev_get_floor_sensor_signal();
    if (floor!=-1) {
        if (((direction == DIRN_UP) || (direction == DIRN_DOWN)) && remove_floor(direction, ctrl, floor)) {
            if (direction == DIRN_UP) {
                ctrl.state = UPWAIT;
            } else if (direction == DIRN_DOWN) {
                ctrl.state = DOWNWAIT;
            }
        }
    }
}
bool is_specific_queue_empty(const bool queue[], const int size){
    for(int i = 0; i < size; ++i){
        if (queue[i]){
            return false;
        }
    }
    return true;
}

bool is_queue_empty(elev_motor_direction_t dir, const Controller_t ctrl){
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

State_t up_or_down_from_idle(const Controller_t ctrl){
    bool is_up_empty = is_queue_empty(DIRN_UP, ctrl);
    bool is_down_empty = is_queue_empty(DIRN_DOWN, ctrl);
    if (is_down_empty && is_up_empty){
        return IDLE;
    }
    assert(ctrl.current_floor <= 4 && ctrl.current_floor >= 1);
    switch (ctrl.current_floor){
        case 1:
            return UP;
        case 4:
            return DOWN;
        default:
            if (!is_up_empty){
                return DOWN;
            }
            else {
                return UP;
            }
    }
}
