#pragma once
#include "controller.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Rotates the queues
 * @param ctrl a ControlStruct
 * */
void rotate_queues(Controller_t* ctrl);
/*
 * Adds buttonpresses in the right queue
 * 
 */
void add_floors_in_queue(Controller_t *ctrl);


/**
 * Checks if queue is empty
 * @param queue a bool array to check
 * @param size size of array
 * @return true if queue is empty
 * */
bool is_queue_empty(const bool queue[], const int size);

bool is_all_queues_empty(Controller_t *ctrl);

void clear_orders(Controller_t* ctrl);

/**
 * Takes in button and floor, finds direction and places it in the right queue
 */
void add_button_to_queue(Controller_t *ctrl, elev_button_type_t button, unsigned int floor);

/**
 * Find the floor with order furthest from endpoints. Returns -1 if no orders
 * @param a ControllerStruct
 * @return most extreme floor
 * */
int find_extreme_in_primary(const Controller_t* ctrl);

/**
 * Updates controller state
 * @param ctrl An elevator controller
 * */
void up_or_down_from_idle(Controller_t* ctrl);