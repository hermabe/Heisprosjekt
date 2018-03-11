#pragma once
#include "controller.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Rotates the queues
 * Primary->Extra->Secondary->Primary
 * @param ctrl a ControlStruct
 * */
void rotate_queues(Controller_t* ctrl);

/*
 * Adds buttonpresses in the right queue
 * @param ctrl a ControlStruct
 */
void add_floors_in_queue(Controller_t *ctrl);

/**
 * Checks if queue is empty
 * @param queue a bool array to check
 * @param size size of array
 * @return true if queue is empty
 * */
bool is_queue_empty(const bool queue[], const int size);

/**
 * Checks if all elements in all queues are zero
 * @param ctrl a ControlStruct
 * @return true if all queues are zero
 * */
bool is_all_queues_empty(Controller_t *ctrl);

/**
 * Sets all elements in all queues to zero
 * @param ctrl a ControlStruct
 * */
void clear_orders(Controller_t* ctrl);

/**
 * Adds an order in the right queue
 * @param ctrl a ControlStruct
 * @param button the type of button
 * @param floor the floor the button is on
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