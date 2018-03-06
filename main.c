#include <stdio.h>
#include "controller.h"

int main() {
    Controller_t ctrl;
    startup(&ctrl); // Initialize
    run(&ctrl);
    return 0;
}
