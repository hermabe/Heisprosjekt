#pragma once
#include "elev.h"
#include <stdio.h>


/**
 * Initializes the hardware and puts the elevator in a known position (1st floor)
 */
void startup();



/****
 * Removes floor in the right direction
 * 
 */

void remove_floor(elev_motor_direction_t direction, controller_t ctrl, floor)

/*
* Check if one have reached a floor and checks queues and performes action
*/

void reached_a_floor(elev_motor_direction_t direction, controller_t ctrl)   