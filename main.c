#include <stdio.h>
#include "controller.h"

int main() {
    Controller_t ctrl;
    startup(&ctrl);
    run(&ctrl);
    return 0;
}
