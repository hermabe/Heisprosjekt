#pragma once
#include "elev.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Defines all possible states
 * */
typedef enum tag_states { INITSTATE,
                          IDLESTATE,
                          STOPSTATE,
                          MOVESTATE,
                          WAITSTATE } State_t;

/**
 * Holds all the values for queue-system
 **/
typedef struct tag_controller{
    unsigned int current_floor;
    elev_motor_direction_t direction;
    State_t state;
    bool queues[3][4];
} Controller_t;


void wait_at_floor(Controller_t *ctrl);

/***
 * Resets buttons at a floor
 *
*/
void reset_button_lights_at_floor(int floor);

/***
 * Resets all light except lamp
 *
*/
void reset_button_lights_except_stop_lamp();
/**
 * Rotates the queues
 * @param ctrl a ControlStruct
 * */
void rotate_queues(Controller_t* ctrl);
/*
 * Adds buttonpresses in the right queue
 * 
 */
void add_floorss_in_queue(Controller_t *ctrl);

/***
 * Takes in button and floor, finds direction and places it in the right queue
 */
void add_button_to_queue(Controller_t *ctrl, elev_button_type_t button, unsigned int floor);

/**
 * Checks if queue is empty
 * @param queue a bool array to check
 * @param size size of array
 * @return true if queue is empty
 * */
bool is_queue_empty(const bool queue[], const int size);

bool is_all_queues_empty(Controller_t *ctrl);

/**
 * @param ctrl An elevator controller
 * @return next state (IDLE/UP/DOWN)
 * */
State_t up_or_down_from_idle(const Controller_t ctrl);

/**
 * Initializes the hardware and controlstruct and puts the elevator in a known position (1st floor)
 * @param ctrl a control struct
 */
void startup(Controller_t* ctrl);

/**
 * Initializes the controlstruct
 * @param ctrl a controller
 * @param current_floor the floor of the elevator
 * @param state the state of the elevator
 * */
void initialize_controlstruct(Controller_t *ctrl, unsigned int current_floor, State_t state);

/*
* Updates currentfloor in ctrl and indicator light
* @param ctrl a control struct
* @param floor to update
*/
void update_floor(Controller_t *ctrl, int floor);

/*
Checks stopsignal and changes state if true
*/
void check_stop(Controller_t* ctrl);

/****
 * Removes floor in the right direction 
 * @param ctrl a control struct
 * @param floor floor to remove
 */
bool remove_floor(Controller_t *ctrl, int floor);

/*
* Check if one have reached a floor and checks queues and performes action
* @param ctrl a control struct
*/
void reached_a_floor(Controller_t *ctrl);


/**
 * Find the floor with order furthest from endpoints. Returns -1 if no orders
 * @param a ControllerStruct
 * @return most extreme floor
 * */
int find_extreme_in_primary(const Controller_t* ctrl);

/**
 * Handles orders and controls the elevator
 * @param ctrl a ControllerStuct
 * */
void run(Controller_t* ctrl);

void toggle_direction(Controller_t* ctrl);

void clear_orders(Controller_t* ctrl);
