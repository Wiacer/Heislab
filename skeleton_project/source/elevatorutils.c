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

void complete_order(const int floor, int orders[4][2]) {

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
    elevio_doorOpenLamp(0);

}
/*
void stop_state(const int floor, ElevState* state, int orders[4][2]) {

    elevio_motorDirection(DIRN_STOP);

    *state = MV_IDLE;

    for (int f = 0; f < 4; f++) {
        for (int o = 0; o < 2; o++) {
            orders[f][o] = 0;
            elevio_buttonLamp(f, 0, 0);
            elevio_buttonLamp(f, 1, 0);
            elevio_buttonLamp(f, 2, 0);
        }
    }
    while (elevio_stopButton()) {
        if (floor != -1) {
            elevio_doorOpenLamp(1);
        }
    }
    if (floor != -1) {
        complete_order(floor, orders);
    }
}*/

void check_btn_inputs(const int floor, int orders[4][2]) {
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            if (btnPressed) {
                elevio_buttonLamp(f,b,1);
                if (b != 2) {
                    orders[f][b] = 1;
                } else {
                    if (floor < f) {
                        orders[f][0] = 1;
                    } else if (floor > f) {
                        orders[f][1] = 1;
                    } else {
                        orders[f][0] = 1;
                        orders[f][1] = 1;
                    }
                }
            }
        }
    }
}

bool check_floor(const int orders[4][2], const int floor) {
    for (int f = 0; f < 4; f++) {
        for (int o = 0; o < 2; o++) {
            if (orders[f][o] == 1 && f == floor) {
                return true;
            }
        }
    }
    return false;
}



