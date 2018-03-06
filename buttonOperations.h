#include "elev.h"
#include <stdio.h>
#include "io.h"
#include "channels.h"

/****************
 * Read buttons and add to queue
 * 
 * ****************/
int read_buttons_and_light_up_buttos();

/***
 * Resets buttons at a floor
 *
*/
void reset_button_lights_at_floor(int floor);

/***
 * Resets all light except lamp
 *
*/
void reset_button_lights_except_stop_lamp();