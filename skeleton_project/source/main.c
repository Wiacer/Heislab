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