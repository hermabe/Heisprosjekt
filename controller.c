#include "controller.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>

void startup(Controller_t *ctrl)
{
    printf("Initializing elevator\n");
    // Initialize hardware
    if (!elev_init())
    {
        printf("Unable to initialize elevator hardware!\n");
        exit(1);
    }

    initialize_controlstruct(ctrl, 0, IDLESTATE);

    // Put elevator in 1st floor
    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal() != (int)0) {}
    elev_set_motor_direction(DIRN_STOP);
    printf("Initialization done\n");
}

void wait_at_floor(Controller_t *ctrl) {
    elev_set_motor_direction(DIRN_STOP);
    elev_set_door_open_lamp(1);
    int msec = 0;
    int trigger = 3000;
    clock_t before = clock();
    clock_t difference;
    do {
        if (check_stop(ctrl)){
            return;
        }        
        read_buttons_and_light_up_button();
        add_floors_in_queue(ctrl);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
    } while (msec < trigger);
    elev_set_door_open_lamp(0);
    ctrl->state = IDLESTATE;
}

void update_floor(Controller_t *ctrl, int floor)
{
    if (floor != -1)
    {
        ctrl->current_floor = floor;
        elev_set_floor_indicator(floor);
    }
}

bool remove_floor(Controller_t *ctrl, int floor)
{    
    if (ctrl->queues[0][floor]){
        for (int queue = 0; queue < 3; queue++) {
            ctrl->queues[queue][floor] = 0;
        }   
        return true;
    } else {
        return false;
    }
}

void reset_button_lights_at_floor(int floor) {
    for (elev_button_type_t button = 0; button < 3; ++button) {
        elev_set_button_lamp(button, floor, 0);
    }
}

void reset_all_lights_except_stop_light() {
    for (int floor=0; floor < 4; floor++) {
        reset_button_lights_at_floor(floor);
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
                } else {
                    ctrl->queues[1][floor] = true; //add in secondary queue
                }
            }
        }
        case BUTTON_CALL_DOWN: {
            if (floor_above_elevator) {
                if (ctrl->direction > 0) {
                    ctrl->queues[1][floor] = true; 
                } else {
                    ctrl->queues[2][floor] = true;
                }
            } else {
                
                if (ctrl->direction <= 0) {
                    ctrl->queues[0][floor] = true; 
                } else { 
                    ctrl->queues[1][floor] = true; 
                }
            }
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

bool if_reached_a_floor_stop(Controller_t *ctrl)
{
    int floor = elev_get_floor_sensor_signal();
    update_floor(ctrl, floor);
    if (ctrl->state == MOVESTATE && remove_floor(ctrl, floor))
    {
        reset_button_lights_at_floor(floor);
        ctrl->state = WAITSTATE;
        return true;
    }
    return false;
}

bool is_queue_empty(const bool queue[], const int size)
{
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

void up_or_down_from_idle(Controller_t* ctrl)
{
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

bool check_stop(Controller_t* ctrl){
    if (elev_get_stop_signal()) {
        ctrl->state = STOPSTATE;
        return true;
    }
    return false;
}

void initialize_controlstruct(Controller_t *ctrl, unsigned int current_floor, State_t state) {
    ctrl->current_floor = current_floor;
    ctrl->state = state;
    ctrl->direction = DIRN_STOP;

    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 4; ++j){
            ctrl->queues[i][j] = 0;
        }
    }
}

void rotate_queues(Controller_t* ctrl){
    bool temp;
    for (int i = 0; i < 4; ++i){
        temp = ctrl->queues[0][i];
        ctrl->queues[0][i] = ctrl->queues[1][i];
        ctrl->queues[1][i] = ctrl->queues[2][i];
        ctrl->queues[0][i] = temp;
    }
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

void activate_stop(Controller_t* ctrl){
    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);
    reset_all_lights_except_stop_light();
    clear_orders(ctrl);
    if (elev_get_floor_sensor_signal() != -1)
    {
        elev_set_door_open_lamp(1);
    }
    while (elev_get_stop_signal())
    {
    }
    elev_set_stop_lamp(0);
    ctrl->state = IDLESTATE;
}

void run(Controller_t* ctrl){
    while(true){
        check_stop(ctrl);        
        read_buttons_and_light_up_button();
        add_floors_in_queue(ctrl);

        switch (ctrl->state){
            case STOPSTATE:
                activate_stop(ctrl);
                break;  
            case IDLESTATE:
                up_or_down_from_idle(ctrl);             
                break;
            case MOVESTATE:
                if_reached_a_floor_stop(ctrl);
                break;
            case WAITSTATE:
                wait_at_floor(ctrl);                
                break;
            default:
                break;
        }
    }
}

void toggle_direction(Controller_t* ctrl){
    if (ctrl->direction == DIRN_UP){
        ctrl->direction = DIRN_DOWN;
        elev_set_motor_direction(DIRN_DOWN);
    }
    else if (ctrl->direction == DIRN_DOWN){
        ctrl->direction = DIRN_UP;
        elev_set_motor_direction(DIRN_UP);
    }
}

void clear_orders(Controller_t* ctrl){
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 4; ++j){
            ctrl->queues[i][j] = 0;
        }
    }
}

void open_door_if_at_floor(){
    if (elev_get_floor_sensor_signal() == -1){
        elev_set_door_open_lamp(1);
    }
}

elev_motor_direction_t get_direction_from_current_and_destination_floor(Controller_t* ctrl, int floor){    
    if (ctrl->current_floor < floor){
        return DIRN_UP;
    }
    else if (ctrl->current_floor > floor){
        return DIRN_DOWN;
    }
    else {
        return (elev_motor_direction_t)((int)(ctrl->direction) * -1);
    }
}

void print_queue(Controller_t *ctrl){
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 3; ++j){
            printf("%d ", ctrl->queues[j][i]);
        }
        printf("\n");
    }
}