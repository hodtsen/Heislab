#include "elevator_controller.h"
#include "lamps.h"
#include "requests.h"
#include "timer_utils.h"

void controller_update_floor(ElevatorController* controller) {
    int sensed_floor = elevio_floorSensor();
    controller->current_floor = sensed_floor;
    if (sensed_floor >= 0) {
        controller->last_known_floor = sensed_floor;
        elevio_floorIndicator(sensed_floor);
    }
}

void controller_start_door_open(ElevatorController* controller) {
    controller->state = STATE_DOOR_OPEN;
    controller->direction = DIRN_STOP;
    elevio_motorDirection(DIRN_STOP);
    elevio_doorOpenLamp(1);
}

void controller_enter_stopped_state(ElevatorController* controller) {
    requests_clear_all(controller);
    lamps_sync_request_lamps(controller);
    controller->state = STATE_STOPPED;
    controller->direction = DIRN_STOP;
    elevio_motorDirection(DIRN_STOP);
    elevio_stopLamp(1);
    elevio_doorOpenLamp(controller->current_floor >= 0);
}

void controller_handle_init(ElevatorController* controller) {
    controller->last_travel_direction = DIRN_DOWN;
    elevio_motorDirection(DIRN_DOWN);
    if (controller->current_floor >= 0) {
        controller->direction = DIRN_STOP;
        controller->state = STATE_IDLE;
        elevio_motorDirection(DIRN_STOP);
    }
}

void controller_handle_idle(ElevatorController* controller) {
    if (elevio_stopButton()) {
        controller_enter_stopped_state(controller);
        return;
    }

    requests_poll(controller);
    lamps_sync_request_lamps(controller);

    if (controller->current_floor >= 0 && requests_exist_at_floor(controller, controller->current_floor)) {
        requests_clear_at_floor(controller, controller->current_floor);
        lamps_sync_request_lamps(controller);
        controller_start_door_open(controller);
        return;
    }

    MotorDirection next_direction = requests_choose_direction(controller);
    if (next_direction == DIRN_STOP) {
        elevio_motorDirection(DIRN_STOP);
        return;
    }

    controller->direction = next_direction;
    controller->last_travel_direction = next_direction;
    controller->state = STATE_MOVING;
    elevio_motorDirection(next_direction);
}

void controller_handle_moving(ElevatorController* controller) {
    if (elevio_stopButton()) {
        controller_enter_stopped_state(controller);
        return;
    }

    requests_poll(controller);
    lamps_sync_request_lamps(controller);

    if (controller->current_floor < 0) {
        return;
    }

    if (requests_should_stop(controller, controller->current_floor)) {
        requests_clear_at_floor(controller, controller->current_floor);
        lamps_sync_request_lamps(controller);
        controller_start_door_open(controller);
        return;
    }

    MotorDirection next_direction = requests_choose_direction(controller);
    if (next_direction != DIRN_STOP && next_direction != controller->direction) {
        controller->direction = next_direction;
        controller->last_travel_direction = next_direction;
        elevio_motorDirection(next_direction);
        return;
    }

    if (next_direction == DIRN_STOP) {
        controller->direction = DIRN_STOP;
        controller->state = STATE_IDLE;
        elevio_motorDirection(DIRN_STOP);
    }
}

void controller_handle_door_open(ElevatorController* controller) {
    if (elevio_stopButton()) {
        controller_enter_stopped_state(controller);
        return;
    }

    for (int elapsed_ms = 0; elapsed_ms < DOOR_OPEN_TIME_MS; elapsed_ms += POLL_INTERVAL_MS) {
        requests_poll(controller);
        lamps_sync_request_lamps(controller);

        if (elevio_stopButton()) {
            controller_enter_stopped_state(controller);
            return;
        }

        if (elevio_obstruction()) {
            return;
        }

        timer_sleep_ms(POLL_INTERVAL_MS);
    }

    elevio_doorOpenLamp(0);
    controller->direction = requests_choose_direction(controller);
    if (controller->direction == DIRN_STOP) {
        controller->state = STATE_IDLE;
        elevio_motorDirection(DIRN_STOP);
        return;
    }

    controller->state = STATE_MOVING;
    controller->last_travel_direction = controller->direction;
    elevio_motorDirection(controller->direction);
}

void controller_handle_stopped(ElevatorController* controller) {
    elevio_motorDirection(DIRN_STOP);
    elevio_stopLamp(1);

    if (elevio_stopButton()) {
        return;
    }

    elevio_stopLamp(0);
    if (controller->current_floor >= 0) {
        controller_start_door_open(controller);
        return;
    }

    elevio_doorOpenLamp(0);
    controller->state = STATE_IDLE;
}

void elevator_controller_init(ElevatorController* controller) {
    controller->state = STATE_INIT;
    controller->direction = DIRN_DOWN;
    controller->last_travel_direction = DIRN_DOWN;
    controller->current_floor = -1;
    controller->last_known_floor = 0;
    requests_clear_all(controller);
}

void elevator_controller_step(ElevatorController* controller) {
    controller_update_floor(controller);

    switch (controller->state) {
        case STATE_INIT:
            controller_handle_init(controller);
            break;
        case STATE_IDLE:
            controller_handle_idle(controller);
            break;
        case STATE_MOVING:
            controller_handle_moving(controller);
            break;
        case STATE_DOOR_OPEN:
            controller_handle_door_open(controller);
            break;
        case STATE_STOPPED:
            controller_handle_stopped(controller);
            break;
        default:
            elevator_controller_init(controller);
            lamps_clear_all();
            break;
    }
}
