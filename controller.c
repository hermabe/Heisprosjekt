#include "controller.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>

void startup(Controller_t *ctrl) {
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
        read_buttons_and_light_up_buttons();
        add_floors_in_queue(ctrl);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
    } while (msec < trigger);
    elev_set_door_open_lamp(0);
    ctrl->state = IDLESTATE;
}

void update_floor(Controller_t *ctrl, int floor) {
    if (floor != -1)
    {
        ctrl->current_floor = floor;
        elev_set_floor_indicator(floor);
    }
}

bool remove_floor(Controller_t *ctrl, int floor) {    
    if (ctrl->queues[0][floor]){
        for (int queue = 0; queue < 3; queue++) {
            ctrl->queues[queue][floor] = 0;
        }   
        return true;
    } else {
        return false;
    }
}

bool stop_if_reached_a_floor(Controller_t *ctrl) {
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

bool check_stop(Controller_t* ctrl){
    if (elev_get_stop_signal()) {
        ctrl->state = STOPSTATE;
        return true;
    }
    return false;
}

void activate_stop(Controller_t* ctrl){
    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);
    reset_button_lights_except_stop_light();
    clear_orders(ctrl);
    open_door_if_at_floor()
    
    while (elev_get_stop_signal())
    {
    }
    elev_set_stop_lamp(0);
    ctrl->state = IDLESTATE;
}

void run(Controller_t* ctrl){
    while(true){
        check_stop(ctrl);        
        read_buttons_and_light_up_buttons();
        add_floors_in_queue(ctrl);

        switch (ctrl->state){
            case STOPSTATE:
                activate_stop(ctrl);
                break;  
            case IDLESTATE:
                decide_next_elev_movement_from_queue(ctrl);      
                break;
            case MOVESTATE:
                stop_if_reached_a_floor(ctrl);
                break;
            case WAITSTATE:
                wait_at_floor(ctrl);                
                break;
            default:
                //Not supposed to happen, but if it does, it stops the elevator, because something is wrong
                ctrl->state = STOPSTATE
                break;
        }
    }
}

void toggle_direction(Controller_t* ctrl){
    if (ctrl->direction == DIRN_UP){
        ctrl->direction = DIRN_DOWN;
    }
    else if (ctrl->direction == DIRN_DOWN){
        ctrl->direction = DIRN_UP;
    }
}

void open_door_if_at_floor(){
    //If at floor
    if (elev_get_floor_sensor_signal() != -1){
        //open door
        elev_set_door_open_lamp(1);
    }
}

elev_motor_direction_t get_direction_from_current_and_destination_floor(Controller_t* ctrl, int destination_floor){    
    if (ctrl->current_floor < destination_floor){
        return DIRN_UP;
    }
    else if (ctrl->current_floor > destination_floor){
        return DIRN_DOWN;
    }
    else {
        //Only happens if stop button is pressed and the elevator has just left destination floor.
        //It then goes the opposite direction to return to destination floor.
        return (elev_motor_direction_t)((int)(ctrl->direction) * -1);
    }
}
