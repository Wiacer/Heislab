#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevatorutils.h"

void check_new_orders(struct ProgramState* programState){
    for (int f = 0; f < N_FLOORS; f++) {
        if (any_true(programState->orders[f],N_BUTTONS)) {
            if((f == programState->lastFloor) && (programState->elevatorState == STOP_IDLE)){
                if(programState->previousState == MV_DWN){
                    programState->lastFloor -= 1;
                    programState->elevatorState = MV_UP;
                    break;
                }else if(programState->previousState == MV_UP){
                    programState->lastFloor += 1;
                    programState->elevatorState = MV_DWN;
                    break;
                }
            }else if (f < programState->lastFloor) {
                programState->elevatorState = MV_DWN;
            } else if (f > programState->lastFloor) {
                programState->elevatorState = MV_UP;
            }
        }
    }
}

void elevator_initialize(struct ProgramState* programState){
    elevio_doorOpenLamp(0);
    elevio_motorDirection(DIRN_DOWN);
    while (elevio_floorSensor() == -1) {
        continue;
    }
    programState->floor = elevio_floorSensor();
    programState->lastFloor = programState->floor;
    programState->floorLight = programState->floor;
    programState->elevatorState = MV_IDLE;
    programState->previousState = MV_IDLE;
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            programState->orders[i][j] = 0;
        }
    }
    elevio_floorIndicator(programState->floorLight);
    elevio_motorDirection(DIRN_STOP);
}

int main(){
    elevio_init();

    struct ProgramState programState;
    
    programState.elevatorState = INIT;

    while(1){
        switch (programState.elevatorState) {
            case INIT:
                elevator_initialize(&programState);
                printf("Initialization done\n");
                break;

            case MV_DWN:
                elevio_motorDirection(DIRN_DOWN);
                while(programState.elevatorState == MV_DWN){
                    update_program_state(&programState);
                    check_orders(&programState);
                    check_stop(&programState);
                }
                break;

            case MV_IDLE:
                elevio_motorDirection(DIRN_STOP);
                while(programState.elevatorState == MV_IDLE){
                    update_program_state(&programState);
                    if (any_true(programState.orders[programState.floor],N_BUTTONS)){
                        complete_order(&programState);
                    }
                    check_new_orders(&programState);
                    check_stop(&programState);
                }
                break;

            case MV_UP:
                elevio_motorDirection(DIRN_UP);
                while(programState.elevatorState == MV_UP){
                    update_program_state(&programState);
                    check_orders(&programState);
                    check_stop(&programState);
                }
                break;

            case MV_STOP:
                stop_init(&programState);
                if(programState.floor != -1){
                    complete_order(&programState);
                    programState.elevatorState = MV_IDLE;
                }else{
                    programState.elevatorState = STOP_IDLE;
                }
                break;

            case STOP_IDLE:
                while(programState.elevatorState == STOP_IDLE){
                    update_program_state(&programState);
                    check_new_orders(&programState);
                    check_stop(&programState);
                }
                break;
            
        }
    }

    return 0;
}