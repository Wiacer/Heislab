#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevatorutils.h"

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

            case MV_IDLE:
                elevio_motorDirection(DIRN_STOP);
                while(programState.elevatorState == MV_IDLE){
                    update_state(&programState);
                }
                break;

            case MV_DWN:
                elevio_motorDirection(DIRN_DOWN);
                while(programState.elevatorState == MV_DWN){
                    update_state(&programState);
                }
                break;

            case MV_UP:
                elevio_motorDirection(DIRN_UP);
                while(programState.elevatorState == MV_UP){
                    update_state(&programState);
                }
                break;

            case STOP:
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
                    update_state(&programState);
                }
                break;
            
        }
    }

    return 0;
}