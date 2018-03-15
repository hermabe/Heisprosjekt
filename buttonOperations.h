#include <stdio.h>
#include "io.h"
#include "elev.h"
#include "channels.h"

/**
 * Read buttons and turn on lights if button pressed
 */
void read_buttons_and_light_up_buttons();

/**
 * Resets buttons at a floor, ie. turns off the lights
 * @param floor the floor to reset all buttons
*/
void reset_button_lights_at_floor(int floor);

/**
 * Resets all light except stop light
*/
void reset_button_lights_except_stop_light();
