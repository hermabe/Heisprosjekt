#include "queue.h"

void rotate_queues(Controller_t* ctrl){
    bool temp;
    // For each floor rotate the elements for that floor in the queues.
    // Primary->Extra->Secondary->Primary
    for (int i = 0; i < 4; ++i){
        temp = ctrl->queues[0][i];
        ctrl->queues[0][i] = ctrl->queues[1][i];
        ctrl->queues[1][i] = ctrl->queues[2][i];
        ctrl->queues[2][i] = temp;
    }
}

void add_floors_in_queue_from_lights(Controller_t *ctrl) {
    for (unsigned int floor = 0; floor < 4; floor++) {
        for (elev_button_type_t button = BUTTON_CALL_UP; button <= BUTTON_COMMAND; button++) {
            // If button has been pressed, the light is on, and the floor should be added in the queue
            if (elev_get_button_lamp(button, floor)) {
                add_button_to_queue(ctrl, button, floor);
            }
        }
    }
}

void add_button_to_queue(Controller_t *ctrl, elev_button_type_t button, unsigned int floor) {
    bool floor_above_elevator = (floor > ctrl->current_floor);

    // Look-up-table instead of lots of ifs
    // 0 is primary queue, 1 is secondary queue, 2 is extra queue
    int queueSelectorArray[18] = {1, 0, 2, 1, 0, 0, 0, 0, 1, 2, 0, 1, 0, 0, 1, 1, 0, 0};
    int index = button*6 + floor_above_elevator*3 + 1 + ctrl->direction;
    int queue = queueSelectorArray[index];
    ctrl->queues[queue][floor] = true;
}

bool is_queue_empty(const bool queue[], const int size) {
    for (int i = 0; i < size; ++i) {
        if (queue[i]) {
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
    // If elevator is going down, return floor closest to 1 that has an order
    if (ctrl->direction == DIRN_DOWN){
        for(int i = 0; i < 4; ++i) {
            if (ctrl->queues[0][i]) {
                return i;
            }
        }
    }
    // Else return floor closest to 4 that has an order
    else {
        for(int i = 3; i >= 0; --i) {
            if (ctrl->queues[0][i]) {
                return i;
            }
        }
    }
    // If no orders return -1
    return -1;
}

void clear_orders(Controller_t* ctrl){
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 4; ++j){
            ctrl->queues[i][j] = 0;
        }
    }
}

void decide_next_elev_movement_from_queue(Controller_t* ctrl) {
    // If there are no orders, elevator should not do anything
    if (is_all_queues_empty(ctrl)){
        ctrl->state = IDLESTATE;
    }
    // If primary queue is empty, rotate the queues so that the secondary queue is checked next
    else if (is_queue_empty(ctrl->queues[0], 4)) {
        rotate_queues(ctrl);
        toggle_direction(ctrl);
    }
    // Go in direction of most extreme order in primary queue
    else {
        int extreme = find_extreme_in_primary(ctrl);
        elev_motor_direction_t direction = get_direction_from_current_and_destination_floor(ctrl, extreme);
        elev_set_motor_direction(direction);
        ctrl->direction = direction;
        ctrl->state = MOVESTATE;        
    }
}
