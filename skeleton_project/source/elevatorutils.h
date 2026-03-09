#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

bool is_in_list(const int list[],const int element, const int len);

void bubble_sort(int list[], const int len);

bool any_true(int* arr, int len);

typedef enum {MV_DWN, MV_IDLE, MV_UP, MV_STOP, STOP_IDLE, INIT} ElevState;

struct ProgramState{
    ElevState elevatorState;
    ElevState previousState;
    int floor;
    int lastFloor;
    int floorLight;
    int orders[N_FLOORS][N_BUTTONS];
};

void complete_order(struct ProgramState* programState);

void check_btn_inputs(struct ProgramState* programState);

bool check_floor(struct ProgramState* programState);

void update_program_state(struct ProgramState* programState);

void check_stop(struct ProgramState* programState);

void check_orders(struct ProgramState* programState);

void stop_init(struct ProgramState* programState);

void check_new_orders(struct ProgramState* programState);

void elevator_initialize(struct ProgramState* programState);