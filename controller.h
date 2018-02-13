#pragma once
#include "elev.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Holds all the values for queue-system
 **/

typedef struct tag_controller{
    unsigned int current_floor;
    bool up_queue[4];
    bool down_queue[4];
} Controller;


bool is_queue_empty(elev_motor_direction_t dir, const Controller ctrl);

/**
 * Initializes the hardware and puts the elevator in a known position (1st floor)
 */
void startup();