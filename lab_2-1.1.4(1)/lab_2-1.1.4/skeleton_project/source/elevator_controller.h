#pragma once
#include <stdbool.h>
#include "driver/elevio.h"

#define POLL_INTERVAL_MS 100
#define DOOR_OPEN_TIME_MS 3000

typedef enum {
    STATE_INIT = 0,
    STATE_IDLE,
    STATE_MOVING,
    STATE_DOOR_OPEN,
    STATE_STOPPED
} ElevatorState;

typedef struct {
    ElevatorState state;
    MotorDirection direction;
    MotorDirection last_travel_direction;
    int current_floor;
    int last_known_floor;
    bool requests[N_FLOORS][N_BUTTONS];
} ElevatorController;

void controller_update_floor(ElevatorController* controller);
void controller_start_door_open(ElevatorController* controller);
void controller_enter_stopped_state(ElevatorController* controller);
void controller_handle_init(ElevatorController* controller);
void controller_handle_idle(ElevatorController* controller);
void controller_handle_moving(ElevatorController* controller);
void controller_handle_door_open(ElevatorController* controller);
void controller_handle_stopped(ElevatorController* controller);
void elevator_controller_init(ElevatorController* controller);
void elevator_controller_step(ElevatorController* controller);
