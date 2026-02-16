#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevatorutils.h"

int main(){
    
    elevio_init();
    int last_state = DIRN_STOP;

    ElevState state = MV_IDLE;
    ElevState prev_state = MV_IDLE;

    int orders[4][2] = {{0,0},{0,0},{0,0},{0,0}};
    elevio_motorDirection(DIRN_DOWN);
    while (elevio_floorSensor() == -1) {
        continue;
    }

    
    int floor = elevio_floorSensor();
    int lfloor = floor;
    int next_stop;

    printf("Initialization done\n");
    elevio_motorDirection(DIRN_STOP);
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    printf("huh??\n");

    while(1){
        
        floor = elevio_floorSensor();
        if (floor != -1) {
            lfloor = floor;
        }

        elevio_floorIndicator(lfloor);

        check_btn_inputs(lfloor, orders);

        if(elevio_stopButton()){
            prev_state = state;
            state = MV_STOP;          
        }

        switch (state) {
            case MV_DWN:

                elevio_motorDirection(DIRN_DOWN);

                next_stop = -1;
                for (int f = 0; f < lfloor; f++) {
                    if (orders[f][1]) {
                        next_stop = f;
                    }
                }

                if (next_stop == -1) {
                    for (int f = 0; f < lfloor; f++) {
                        if (orders[f][0]) {
                            next_stop = f;
                        }
                    }
                }

                if (next_stop == -1) {
                    state = MV_IDLE;
                }

                if (floor != -1) {
                    if (orders[floor][1]) {
                        complete_order(floor, orders);
                    }
                }

                
                break;
            case MV_IDLE:

                elevio_motorDirection(DIRN_STOP);
                
                if (check_floor(orders, floor)) {
                    complete_order(floor, orders);
                }
                
                for (int f = 0; f < 4; f++) {
                    for (int o = 0; o < 2; o++) {
                        if (orders[f][o] == 1) {
                            if (f < floor) {
                                state = MV_DWN;
                            } else if (f > floor) {
                                state = MV_UP;
                            }
                        }
                    }
                }

                printf("idle %d\n", floor);
                break;
            case MV_UP:
                
                elevio_motorDirection(DIRN_UP);

                next_stop = -1;
                for (int f = 3; f > lfloor; f--) {
                    if (orders[f][0]) {
                        next_stop = f;
                    }
                }

                if (next_stop == -1) {
                    for (int f = 3; f > lfloor; f--) {
                        if (orders[f][1]) {
                            next_stop = f;
                        }
                    }
                }

                if (next_stop == -1) {
                    state = MV_IDLE;
                }

                if (floor != -1) {
                    if (orders[floor][0]) {
                        complete_order(floor, orders);
                    }
                }

                break;

            case MV_STOP:
                elevio_motorDirection(DIRN_STOP);
                for (int f = 0; f < 4; f++) {
                    for (int o = 0; o < 2; o++) {
                        orders[f][o] = 0;
                        elevio_buttonLamp(f, 0, 0);
                        elevio_buttonLamp(f, 1, 0);
                        elevio_buttonLamp(f, 2, 0);
                    }
                }
                elevio_stopLamp(1);
                if(floor != -1){
                    elevio_doorOpenLamp(1);
                }
                while(elevio_stopButton());
                if(floor != -1){
                    int duration = 3;
    
                    time_t startTime = time(NULL);
                    time_t endTime = startTime + duration;

                    while (time(NULL) < endTime) {
                        elevio_doorOpenLamp(1);
                        check_btn_inputs(floor, orders);
                        orders[floor][0] = 0;
                        orders[floor][1] = 0;
                        elevio_buttonLamp(floor, 0, 0);
                        elevio_buttonLamp(floor, 1, 0);
                        elevio_buttonLamp(floor, 2, 0);
                        elevio_motorDirection(DIRN_STOP);
                        if (orders[floor][0] || orders[floor][1] || elevio_obstruction()) {
                            endTime = time(NULL) + duration;
                        }
                    }
                }else{
                    floor = elevio_floorSensor();
                    if (floor != -1) {
                        lfloor = floor;
                    }
                    elevio_floorIndicator(lfloor);
                    elevio_motorDirection(DIRN_STOP);
                    while(state == MV_STOP){
                        printf("%u",state);

                        check_btn_inputs(lfloor, orders);


                
                        if (check_floor(orders, floor)) {
                            complete_order(floor, orders);
                        }
                        
                        for (int f = 0; f < 4; f++) {
                            for (int o = 0; o < 2; o++) {
                                if (orders[f][o] == 1) {
                                    if(f == lfloor){
                                        if(prev_state == MV_DWN){
                                            floor += 1;
                                            lfloor += 1;
                                        }else{
                                            floor -= 1;
                                            lfloor -= 1;
                                        }
                                    }
                                    if (f < floor) {
                                        state = MV_DWN;
                                    } else if (f > floor) {
                                        state = MV_UP;
                                    }
                                }
                            }
                        }

                        printf("idle %d\n", floor);
                    }
                    
                }

                break;
            
        }
    }

    return 0;
}