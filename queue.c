#include "queue.h"

void rotate_queues(Controller_t* ctrl){
    bool temp;
    for (int i = 0; i < 4; ++i){
        temp = ctrl->queues[0][i];
        ctrl->queues[0][i] = ctrl->queues[1][i];
        ctrl->queues[1][i] = ctrl->queues[2][i];
        ctrl->queues[2][i] = temp;
    }
}

void add_floors_in_queue(Controller_t *ctrl) {
    for (unsigned int floor = 0; floor < 4;floor ++)
    {
        for (elev_button_type_t button = BUTTON_CALL_UP; button <= BUTTON_COMMAND; button++)
        {
            if (elev_get_button_lamp(button, floor)) {
                add_button_to_queue(ctrl, button, floor);
            }
        }
    }
}

void add_button_to_queue(Controller_t *ctrl, elev_button_type_t button, unsigned int floor) {
    bool floor_above_elevator = (floor > ctrl->current_floor);

    switch (button) {
        case BUTTON_CALL_UP: {
            if (floor_above_elevator) {
                if (ctrl->direction >= 0) {
                    ctrl->queues[0][floor] = true; //Add in primary queue
                } else {
                    ctrl->queues[1][floor] = true; //Add in secondary queue
                }
            } else {
                if (ctrl->direction > 0) {
                    ctrl->queues[2][floor] = true; //Add in extra queue
                } else if (ctrl->direction < 0){
                    ctrl->queues[1][floor] = true; //add in secondary queue
                } else {
                    ctrl->queues[0][floor] = true;
                }
            }
            break;
        }
        case BUTTON_CALL_DOWN: {
            if (floor_above_elevator) {
                if (ctrl->direction > 0) {
                    ctrl->queues[1][floor] = true; 
                } else if (ctrl->direction < 0){
                    ctrl->queues[2][floor] = true;
                } else {
                    ctrl->queues[0][floor] = true;
                }
                
            } else {
                
                if (ctrl->direction <= 0) {
                    ctrl->queues[0][floor] = true; 
                } else { 
                    ctrl->queues[1][floor] = true; 
                }
            }
            break;
        }
        case BUTTON_COMMAND: {
            if (floor_above_elevator) {
                if (ctrl->direction < 0) {
                    ctrl->queues[1][floor] = true;
                } else {
                    ctrl->queues[0][floor] = true;
                }
            } else {
                if (ctrl->direction > 0) {
                    ctrl->queues[1][floor] = true;
                } else {
                    ctrl->queues[0][floor] = true;
                }
            }
        }
    }
}

bool is_queue_empty(const bool queue[], const int size) {
    for (int i = 0; i < size; ++i)
    {
        if (queue[i])
        {
            return false;
        }
    }
    return true;
}

bool is_all_queues_empty(Controller_t *ctrl) {
    for (int queue = 0; queue < 3; queue++) {
        if (!is_queue_empty(ctrl->queues[queue], 4)) {
            return false;
        }
    }
    return true;
}

int find_extreme_in_primary(const Controller_t* ctrl){
    if (ctrl->direction == DIRN_DOWN){
        for(int i = 0; i < 4; ++i){
            if (ctrl->queues[0][i])
            {
                return i;
            }
        }
    }
    else{
        for(int i = 3; i >= 0; --i){
            if (ctrl->queues[0][i]){
                return i;
            }
        }
    }
    return -1;
}

void clear_orders(Controller_t* ctrl){
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 4; ++j){
            ctrl->queues[i][j] = 0;
        }
    }
}

void up_or_down_from_idle(Controller_t* ctrl) {
    if (is_all_queues_empty(ctrl)){
        ctrl->state = IDLESTATE;
    }
    else if (is_queue_empty(ctrl->queues[0], 4)){
        rotate_queues(ctrl);
        toggle_direction(ctrl);
    }
    else {
        int extreme = find_extreme_in_primary(ctrl);
        elev_motor_direction_t direction = get_direction_from_current_and_destination_floor(ctrl, extreme);
        elev_set_motor_direction(direction);
        ctrl->state = MOVESTATE;
        ctrl->direction = direction;             
    }
}