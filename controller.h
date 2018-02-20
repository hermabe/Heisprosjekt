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
                          UPSTATE,
                          DOWNSTATE,
                          UPWAITSTATE,
                          DOWNWAITSTATE } State_t;

/**
 * Holds all the values for queue-system
 **/
typedef struct tag_controller{
    unsigned int current_floor;
    State_t state;
    bool up_queue[4];
    bool down_queue[4];
} Controller_t;

/**
 * Checks if queue is empty
 * @param dir Queue to check up/down/both
 * @param ctrl An elevator controller
 * @return true if queue is empty
 * */
bool is_queue_empty(elev_motor_direction_t dir, const Controller_t ctrl);

/**
 * @param ctrl An elevator controller
 * @return next state (IDLE/UP/DOWN)
 * */
State_t up_or_down_from_idle(const Controller_t ctrl);

/**
 * Initializes the hardware and puts the elevator in a known position (1st floor)
 */
void startup();

/*
Updates currentfloor in ctrl and indicator light
*/
void update_floor(Controller_t *ctrl, int floor);

/*
Checks stopsignal and changes state if true
*/
void check_stop();

/****
 * Removes floor in the right direction
 * 
 */

bool remove_floor(Controller_t *ctrl, int floor);

/*
* Check if one have reached a floor and checks queues and performes action
*/

void reached_a_floor(Controller_t *ctrl);