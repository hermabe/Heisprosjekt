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

    
    // Put elevator in 1st floor
    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal() != (int)0) {}
    elev_set_motor_direction(DIRN_STOP);
    initialize_controlstruct(ctrl, 0, IDLESTATE);
    printf("Initialization done\n");
}

void wait_at_floor(Controller_t *ctrl) {
    //Stop elevator and open door
    elev_set_motor_direction(DIRN_STOP);
    elev_set_door_open_lamp(1);

    //Initialize timer
    int counter = 0;
    int trigger = 3000;
    clock_t before = clock();
    clock_t difference;


    do {
        //Monitor buttons
        if (check_stop(ctrl)){
            return;
        }        
        read_buttons_and_light_up_buttons();
        add_floors_in_queue(ctrl);

        //Update timer
        difference = clock() - before;
        counter = difference * 1000 / CLOCKS_PER_SEC; //Update counter
    } while (counter < trigger);

    //Close door and change to IDLESTATE
    elev_set_door_open_lamp(0);
    ctrl->state = IDLESTATE;
}

void update_floor_indicator(Controller_t *ctrl, int floor) {
    if (floor != -1)
    {
        ctrl->current_floor = floor;
        elev_set_floor_indicator(floor);
    }
}

void stop_if_reached_a_floor(Controller_t *ctrl) {
    int floor = elev_get_floor_sensor_signal();
    update_floor_indicator(ctrl, floor);
    if (remove_floor_from_queue_if_in_primary_queue(ctrl, floor)){
        reset_button_lights_at_floor(floor);
        ctrl->state = WAITSTATE;
    }
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
    //Stop elevator and activate stop lamp
    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);

    //Clears lights and queue
    reset_button_lights_except_stop_light();
    clear_orders(ctrl);


    open_door_if_at_floor();
    
    while (elev_get_stop_signal()) {} //Waits until stop button is released
    elev_set_stop_lamp(0);
    ctrl->state = IDLESTATE;
}

void run(Controller_t* ctrl){
    while(true){
        check_stop(ctrl);        
        read_buttons_and_light_up_buttons();
        add_floors_in_queue_from_lights(ctrl);

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
                ctrl->state = STOPSTATE;
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
