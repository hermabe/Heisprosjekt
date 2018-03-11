#pragma once
#include "elev.h"
#include "buttonOperations.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Defines all possible states
 * */
typedef enum tag_states { IDLESTATE,
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

#include "queue.h" //Controller_t is needed in queue 


/**
 * Initializes the hardware and controlstruct and puts the elevator in a known position (1st floor)
 * @param ctrl a control struct
 */
void startup(Controller_t* ctrl);

/*
 * Stops elevator, opens door and waits for three seconds while checking for buttoninput.
 * When three seconds have passed, opens door
 * @param ctrl a control struct
 * */
void wait_at_floor(Controller_t *ctrl);

/*
 * Updates current_floor in ctrl and indicator light
 * @param ctrl a control struct
 * @param floor to update
 * */
void update_floor(Controller_t *ctrl, int floor);

/*
 * Removes floor in all queues if it is in primary queue 
 * @param ctrl a control struct
 * @param floor floor to remove
 */
bool remove_floor(Controller_t *ctrl, int floor);

/*
 * Check if one have reached a floor and checks queues and performes action
 * @param ctrl a control struct
 * @return true if floor removed from queue
 */
bool stop_if_reached_a_floor(Controller_t *ctrl);

/**
 * Initializes the controlstruct
 * @param ctrl a controller
 * @param current_floor the floor of the elevator
 * @param state the state of the elevator
 * */
void initialize_controlstruct(Controller_t *ctrl, unsigned int current_floor, State_t state);

/*
 * Checks stopsignal and changes to STOPSTATE if true
 * @param ctrl a control struct
 * */
bool check_stop(Controller_t* ctrl);

/*
 * Stops motor, sets stoplamp, resets lights, clear orders and opens door if at floor
 * and waits untill the stop button is released, turns off stop button light and sets state to IDLESTATE
 * @param ctrl a control struct
 * */
void activate_stop(Controller_t* ctrl);


/**
 * Handles orders and controls the elevator
 * @param ctrl a control stuct
 * */
void run(Controller_t* ctrl);

/****
 * Changes to opposite direction in controller struct
 * @param ctrl a control struct
 * */
void toggle_direction(Controller_t* ctrl);

/****
 * Reads floorsensor and opens if it is not -1
 * */
void open_door_if_at_floor();


/****
 * Finds the direction which the elevator has to travel to get to destination floor.
 * @param ctrl a control struct, int destination_floor
 * */
elev_motor_direction_t get_direction_from_current_and_destination_floor(Controller_t* ctrl, int destination_floor);
