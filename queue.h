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