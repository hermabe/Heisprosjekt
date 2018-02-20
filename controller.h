#pragma once
#include "elev.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Defines all possible states
 * */
typedef enum tag_states {INIT, IDLE, STOP, UP, DOWN, UPWAIT, DOWNWAIT} States_t;

/**
 * Holds all the values for queue-system
 **/
typedef struct tag_controller{
    unsigned int current_floor;
    unsigned int state;
    bool up_queue[4];
    bool down_queue[4];
} Controller_t;


bool is_queue_empty(elev_motor_direction_t dir, const Controller_t ctrl);
elev_motor_direction_t up_or_down_from_idle(const Controller_t ctrl);

/**
 * Initializes the hardware and puts the elevator in a known position (1st floor)
 */
void startup();