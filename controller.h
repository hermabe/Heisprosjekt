#pragma once
#include "elev.h"
#include "queue.h"
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
 * Holds all the values required to control elevator
 **/
typedef struct tag_controller{
    unsigned int current_floor;
    elev_motor_direction_t direction;
    State_t state;
    bool queues[3][4];
} Controller_t;


void wait_at_floor(Controller_t *ctrl);

/**
 * @param ctrl An elevator controller
 * @return next state (IDLE/UP/DOWN)
 * */
void up_or_down_from_idle(Controller_t* ctrl);

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
bool check_stop(Controller_t* ctrl);

/****
 * Removes floor in the right direction 
 * @param ctrl a control struct
 * @param floor floor to remove
 */
bool remove_floor(Controller_t *ctrl, int floor);

/*
* Check if one have reached a floor and checks queues and performes action
* @param ctrl a control struct
* @return true if floor removed from queue
*/
bool if_reached_a_floor_stop(Controller_t *ctrl);

/**
 * Handles orders and controls the elevator
 * @param ctrl a ControllerStuct
 * */
void run(Controller_t* ctrl);

void toggle_direction(Controller_t* ctrl);

elev_motor_direction_t get_direction_from_current_and_destination_floor(Controller_t* ctrl, int floor);
