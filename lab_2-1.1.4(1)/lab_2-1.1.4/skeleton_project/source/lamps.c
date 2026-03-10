#include "lamps.h"
#include "requests.h"

void lamps_sync_request_lamps(const ElevatorController* controller) {
    for (int floor = 0; floor < N_FLOORS; ++floor) {
        for (int button = 0; button < N_BUTTONS; ++button) {
            if (!requests_button_is_valid(floor, (ButtonType)button)) {
                continue;
            }
            elevio_buttonLamp(floor, (ButtonType)button, controller->requests[floor][button]);
        }
    }
}

void lamps_clear_all(void) {
    for (int floor = 0; floor < N_FLOORS; ++floor) {
        for (int button = 0; button < N_BUTTONS; ++button) {
            if (!requests_button_is_valid(floor, (ButtonType)button)) {
                continue;
            }
            elevio_buttonLamp(floor, (ButtonType)button, 0);
        }
    }
    elevio_doorOpenLamp(0);
    elevio_stopLamp(0);
}
