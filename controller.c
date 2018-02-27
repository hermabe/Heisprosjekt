#include "controller.h"
#include <assert.h>

void startup(Controller_t *ctrl)
{
    printf("Initializing elevator\n");
    // Initialize hardware
    if (!elev_init())
    {
        printf("Unable to initialize elevator hardware!\n");
        exit(1);
    }

    initialize_controlstruct(ctrl, 0, IDLESTATE);

    // Put elevator in 1st floor
    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal() != (int)0)
    {
        if (elev_get_stop_signal())
        {
            elev_set_motor_direction(DIRN_STOP);
            printf("Startup aborted\n");
            exit(0);
        }
    }
    elev_set_motor_direction(DIRN_UP);
    elev_set_motor_direction(DIRN_STOP);
    printf("Initialization done\n");
}

void update_floor(Controller_t *ctrl, int floor)
{
    if (floor != -1)
    {
        printf("%d\n", floor);
        ctrl->current_floor = floor;
        elev_set_floor_indicator(floor);
    }
}

bool remove_floor(Controller_t *ctrl, int floor)
{
    if ((ctrl->state == UPSTATE) && ctrl->up_queue[floor] == 1)
    {
        ctrl->up_queue[floor] = 0;
        
        return true;
    }
    else if ((ctrl->state == DOWNSTATE) && ctrl->down_queue[floor] == 1)
    {
        ctrl->down_queue[floor] = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void reset_lights(int floor) {
    for (elev_button_type_t button = 0; button++; button<3) {
        elev_set_button_lamp(button, floor, 0);
    }
}

void add_floors(Controller_t *ctrl) {
    elev_motor_direction_t direction = crtl->direction;
    for (unsigned int floor = 0; floor++; floor < 4) {
        for(elev_button_type_t button=BUTTON_CALL_UP; button++; button <= BUTTON_COMMAND) {
            bool above_elevator = false;
            
            button_pressed = elev_get_button_lamp(button, floor);
            if button_pressed {
                add_button_to_queue(ctrl, button, floor)
            }
        }
    }
}

void add_button_to_queue(Controller_t *ctrl, elev_button_type_t button, unsigned int floor) {
    if (floor<ctrl->current_floor) {
        above_elevator = true;
    }
    switch (button) {
        case BUTTON_CALL_UP: {
            if above_elevator {
                if direction >= 0 {
                    ctrl->queues[0][floor] = true; //Add in primary queue
                } else {
                    ctrl->queues[2][floor] = true; //Add in extra queue
                }
            } else {
                if direction > 0 {
                    ctrl->queues[1][floor] = true; //Add in secondary queue
                }
            }
        }
        case BUTTON_CALL_DOWN: {
            if above_elevator {
                if direction > 0 {
                    ctrl->queues[1][floor] = true; 
                }
            } else {
                
                if direction <= 0 {
                    ctrl->queues[0][floor] = true; 
                } else { 
                    ctrl->queues[2][floor] = true; 
                }
            }
        }
        case BUTTON_COMMAND: {
            if above_elevator {
                if direction <= 0 {
                    ctrl->queues[0][floor] = true;
                } else {
                    ctrl->queues[2][floor] = true;
                }
            } else {
                if direction > 0 {
                    ctrl->queues[1][floor] = true;
                }
            }
        }
    }
}

void reached_a_floor(Controller_t *ctrl)
{
    int floor = elev_get_floor_sensor_signal();
    update_floor(ctrl, floor);
    if (((ctrl->state == UPSTATE) || (ctrl->state == DOWNSTATE)) && remove_floor(ctrl, floor))
    {
        reset_lights(floor);
        if (ctrl->state == UPSTATE)
        {
            ctrl->state = UPWAITSTATE;
        }
        else if (ctrl->state == DOWNSTATE)
        {
            ctrl->state = DOWNWAITSTATE;
        }
    }
}

bool is_queue_empty(const bool queue[], const int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (queue[i])
        {
            return false;
        }
    }
    return true;
}

// bool is_queue_empty(elev_motor_direction_t dir, const Controller_t ctrl)
// {
//     if (dir == DIRN_DOWN)
//     {
//         return is_specific_queue_empty(ctrl.down_queue, N_FLOORS);
//     }
//     else if (dir == DIRN_UP)
//     {
//         return is_specific_queue_empty(ctrl.up_queue, N_FLOORS);
//     }
//     else
//     {
//         return is_specific_queue_empty(ctrl.down_queue, N_FLOORS) && is_specific_queue_empty(ctrl.up_queue, N_FLOORS);
//     }
// }

State_t up_or_down_from_idle(const Controller_t ctrl)
{
    bool is_up_empty = is_queue_empty(DIRN_UP, ctrl);
    bool is_down_empty = is_queue_empty(DIRN_DOWN, ctrl);
    if (is_down_empty && is_up_empty)
    {
        return IDLESTATE;
    }
    assert(ctrl.current_floor <= 4 && ctrl.current_floor >= 1);
    switch (ctrl.current_floor)
    {
    case 1:
        return UPSTATE;
    case 4:
        return DOWNSTATE;
    default:
        if (!is_up_empty)
        {
            return DOWNSTATE;
        }
        else
        {
            return UPSTATE;
        }
    }
}

void check_stop(Controller_t* ctrl){
    bool is_stop_pressed = elev_get_stop_signal();
    elev_set_stop_lamp(is_stop_pressed);
    if (is_stop_pressed){
        ctrl->state = STOPSTATE;
    }
}

void initialize_controlstruct(Controller_t *ctrl, unsigned int current_floor, State_t state) {
    ctrl->current_floor = current_floor;
    ctrl->state = state;
    ctrl->direction = DIRN_STOP;

    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 4; ++j){
            ctrl->queues[i][j] = 0;
        }
    }
}

void rotate_queues(Controller_t* ctrl){
    bool** primary = &ctrl->queues[0];
    bool** secondary = &ctrl->queues[1];
    bool** extra = &ctrl->queues[2];

    bool** temp = *primary;
    *primary = *secondary;
    *secondary = *extra;
    *extra = *temp;
}