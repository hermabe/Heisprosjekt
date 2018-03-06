#include "elev.h"
#include <stdio.h>
#include "io.h"
#include "channels.h"
#include "controller.h"

/**
 * Read buttons and add to queue
 */
void read_buttons_and_light_up_buttons();

/**
 * Resets buttons at a floor
 * @param floor the floor to reset all buttons
*/
void reset_button_lights_at_floor(int floor);

/**
 * Resets all light except lamp
*/
void reset_button_lights_except_stop_lamp();
