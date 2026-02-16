#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

bool is_in_list(const int list[],const int element, const int len);

void bubble_sort(int list[], const int len);

void complete_order(const int floor, int orders[4][2]);

typedef enum {MV_DWN, MV_IDLE, MV_UP, MV_STOP} ElevState;

void check_btn_inputs(const int floor, int orders[4][2]);

bool check_floor(const int orders[4][2], const int floor);

void stop_state(const int floor, ElevState* state, int orders[4][2]);