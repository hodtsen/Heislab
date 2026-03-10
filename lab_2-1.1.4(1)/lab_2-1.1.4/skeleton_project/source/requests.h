#pragma once
#include <stdbool.h>
#include "elevator_controller.h"

MotorDirection opposite_direction(MotorDirection direction);
bool requests_button_is_valid(int floor, ButtonType button);
void requests_clear_all(ElevatorController* controller);
void requests_poll(ElevatorController* controller);
bool requests_exist_at_floor(const ElevatorController* controller, int floor);
bool requests_exist_above(const ElevatorController* controller, int floor);
bool requests_exist_below(const ElevatorController* controller, int floor);
bool requests_should_stop(const ElevatorController* controller, int floor);
void requests_clear_at_floor(ElevatorController* controller, int floor);
MotorDirection requests_choose_direction(const ElevatorController* controller);
