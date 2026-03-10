#include "requests.h" 

MotorDirection opposite_direction(MotorDirection direction) {
    if (direction == DIRN_UP) {
        return DIRN_DOWN;
    }
    if (direction == DIRN_DOWN) {
        return DIRN_UP;
    }
    return DIRN_STOP;
}

bool requests_button_is_valid(int floor, ButtonType button) {
    if (button == BUTTON_HALL_UP) {
        return floor < N_FLOORS - 1;
    }
    if (button == BUTTON_HALL_DOWN) {
        return floor > 0;
    }
    return true;
}

void requests_clear_all(ElevatorController* controller) {
    for (int floor = 0; floor < N_FLOORS; ++floor) {
        for (int button = 0; button < N_BUTTONS; ++button) {
            controller->requests[floor][button] = false;
        }
    }
}

void requests_poll(ElevatorController* controller) {
    for (int floor = 0; floor < N_FLOORS; ++floor) {
        for (int button = 0; button < N_BUTTONS; ++button) {
            if (!requests_button_is_valid(floor, (ButtonType)button)) {
                continue;
            }
            if (elevio_callButton(floor, (ButtonType)button)) {
                controller->requests[floor][button] = true;
            }
        }
    }
}

bool requests_exist_at_floor(const ElevatorController* controller, int floor) {
    for (int button = 0; button < N_BUTTONS; ++button) {
        if (requests_button_is_valid(floor, (ButtonType)button) && controller->requests[floor][button]) {
            return true;
        }
    }
    return false;
}

bool requests_exist_above(const ElevatorController* controller, int floor) {
    for (int scan_floor = floor + 1; scan_floor < N_FLOORS; ++scan_floor) {
        if (requests_exist_at_floor(controller, scan_floor)) {
            return true;
        }
    }
    return false;
}

bool requests_exist_below(const ElevatorController* controller, int floor) {
    for (int scan_floor = floor - 1; scan_floor >= 0; --scan_floor) {
        if (requests_exist_at_floor(controller, scan_floor)) {
            return true;
        }
    }
    return false;
}

bool requests_should_stop(const ElevatorController* controller, int floor) {
    if (controller->requests[floor][BUTTON_CAB]) {
        return true;
    }

    if (controller->direction == DIRN_UP) {
        if (requests_button_is_valid(floor, BUTTON_HALL_UP) && controller->requests[floor][BUTTON_HALL_UP]) {
            return true;
        }
        if (!requests_exist_above(controller, floor) &&
            requests_button_is_valid(floor, BUTTON_HALL_DOWN) &&
            controller->requests[floor][BUTTON_HALL_DOWN]) {
            return true;
        }
    }

    if (controller->direction == DIRN_DOWN) {
        if (requests_button_is_valid(floor, BUTTON_HALL_DOWN) && controller->requests[floor][BUTTON_HALL_DOWN]) {
            return true;
        }
        if (!requests_exist_below(controller, floor) &&
            requests_button_is_valid(floor, BUTTON_HALL_UP) &&
            controller->requests[floor][BUTTON_HALL_UP]) {
            return true;
        }
    }

    if (controller->direction == DIRN_STOP) {
        return requests_exist_at_floor(controller, floor);
    }

    return false;
}

void requests_clear_at_floor(ElevatorController* controller, int floor) {
    for (int button = 0; button < N_BUTTONS; ++button) {
        if (!requests_button_is_valid(floor, (ButtonType)button)) {
            continue;
        }
        controller->requests[floor][button] = false;
    }
}

MotorDirection requests_choose_direction(const ElevatorController* controller) {
    int floor = controller->last_known_floor;

    if (controller->current_floor < 0 && requests_exist_at_floor(controller, floor)) {
        return opposite_direction(controller->last_travel_direction);
    }

    if (controller->direction == DIRN_UP) {
        if (requests_exist_above(controller, floor)) {
            return DIRN_UP;
        }
        if (requests_exist_below(controller, floor)) {
            return DIRN_DOWN;
        }
        return DIRN_STOP;
    }

    if (controller->direction == DIRN_DOWN) {
        if (requests_exist_below(controller, floor)) {
            return DIRN_DOWN;
        }
        if (requests_exist_above(controller, floor)) {
            return DIRN_UP;
        }
        return DIRN_STOP;
    }

    if (requests_exist_above(controller, floor)) {
        return DIRN_UP;
    }
    if (requests_exist_below(controller, floor)) {
        return DIRN_DOWN;
    }
    return DIRN_STOP;
}
