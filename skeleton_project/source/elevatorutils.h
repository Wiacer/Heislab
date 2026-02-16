#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

bool is_in_list(const int list[],const int element, const int len);

void bubble_sort(int list[], const int len);

void complete_order();