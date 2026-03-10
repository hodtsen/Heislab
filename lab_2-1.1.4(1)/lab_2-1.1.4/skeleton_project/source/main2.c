#include <stdbool.h>

#include "driver/elevio.h"
#include "elevator_controller.h"
#include "lamps.h"
#include "timer_utils.h"

int main(void) {
    ElevatorController controller;

    elevio_init();
    elevator_controller_init(&controller);
    lamps_clear_all();

    while (true) {
        elevator_controller_step(&controller);
        timer_sleep_ms(POLL_INTERVAL_MS);
    }
}
