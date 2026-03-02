#include "elevatorutils.h"

bool is_in_list(const int list[],const int element, const int len){
    for(int i = 0; i<len; i++){
        if(list[i] == element){
            return true;
        }
    }
    return false;
}

void bubble_sort(int list[], const int len){
    bool unsorted = true;
    int temp;
    while(unsorted){
        bool out_of_order = false;
        for(int i = 0; i<len -1;i++){
            if(list[i]<list[i+1]){
                out_of_order = true;
                temp = list[i];
                list[i] = list[i+1];
                list[i+1] = temp;
            }
        }
        unsorted = out_of_order;
    }
}

bool any_true(int* arr, int len){
    for(int i = 0; i < len; i++){
        if(arr[i]){
            return true;
        }
    }
    return false;
}

void complete_order(struct ProgramState* programState){
    programState->previousState = programState->elevatorState;

    int duration = 3;
    
    time_t startTime = time(NULL);
    time_t endTime = startTime + duration;
    elevio_motorDirection(DIRN_STOP);

    while (time(NULL) < endTime){
        if(elevio_stopButton()){
            programState->elevatorState = MV_STOP;
            break;
        }
        elevio_doorOpenLamp(1);
        for(int i = 0; i < N_BUTTONS; i++){
            programState->orders[programState->floor][i] = 0;
        }
        check_btn_inputs(programState);
        if (any_true(programState->orders[programState->floor],N_BUTTONS) || elevio_obstruction()) {
            endTime = time(NULL) + duration;
        }
    }

    elevio_doorOpenLamp(0);
    switch(programState->previousState){
        case MV_UP:
            elevio_motorDirection(DIRN_UP);
            break;
        case MV_DWN:
            elevio_motorDirection(DIRN_DOWN);
            break;
        default:
            break;
    }
}

void check_btn_inputs(struct ProgramState* programState) {
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            programState->orders[f][b] |= elevio_callButton(f, b);
            elevio_buttonLamp(f,b,programState->orders[f][b]);
        }
    }
}

bool check_floor(struct ProgramState* programState) {
    for (int f = 0; f < N_FLOORS; f++) {
        for (int b = 0; b < N_BUTTONS; b++) {
            if ((programState->orders[f][b] == 1) && (f == programState->floor)) {
                return true;
            }
        }
    }
    return false;
}

void update_program_state(struct ProgramState* programState){
    programState->floor = elevio_floorSensor();
    if (programState->floor != -1) {
        programState->lastFloor = programState->floor;
        programState->floorLight = programState->floor;
    }
    elevio_floorIndicator(programState->floorLight);
    check_btn_inputs(programState);
}

void check_stop(struct ProgramState* programState){
    if(elevio_stopButton()){
        if(programState->elevatorState != STOP_IDLE){
            programState->previousState = programState->elevatorState;
        }
        programState->elevatorState = MV_STOP;
    }
}

void check_orders(struct ProgramState* programState){
    int goingDown = (programState->elevatorState == MV_DWN);

    bool ordersLeft = false;
    for(int b = 0; b < N_BUTTONS; b++){
        if(goingDown){
            for (int f = 0; f < programState->lastFloor; f++) {
                if(programState->orders[f][b]){
                    ordersLeft = true;
                }
            }
        }else{
            for (int f = 3; f > programState->lastFloor; f--) {
                if(programState->orders[f][b]){
                    ordersLeft = true;
                }
            }
        }
    }

    if (!ordersLeft){
        programState->elevatorState = MV_IDLE;
    }

    if (programState->floor != -1) {
        if (programState->orders[programState->floor][goingDown] || 
            programState->orders[programState->floor][2]){
            complete_order(programState);
        }
    }
}

void stop_init(struct ProgramState* programState){
    elevio_motorDirection(DIRN_STOP);
    for (int f = 0; f < N_FLOORS; f++) {
        for (int b = 0; b < N_BUTTONS; b++) {
            programState->orders[f][b] = 0;
            elevio_buttonLamp(f, b, 0);
        }
    }
    elevio_stopLamp(1);
    if(programState->floor != -1){
        elevio_doorOpenLamp(1);
    }
    while(elevio_stopButton());
    elevio_stopLamp(0);
}