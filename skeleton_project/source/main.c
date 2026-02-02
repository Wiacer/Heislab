#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"


struct OrderList{
    int orders[4];
    int last_order_idx;
};

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

void add_order(struct OrderList *order_list,const int order){
    if(!is_in_list(order_list->orders,order,4)){
        order_list->last_order_idx++;
        order_list->orders[order_list->last_order_idx] = order;
        bubble_sort(order_list->orders,4);
    }
}

void complete_order(struct OrderList *order_list,const bool inv){
    if(inv) {
        order_list->orders[0] = -1;
    } else {
        order_list->orders[order_list->last_order_idx] = -1;
    }
    bubble_sort(&order_list->orders,4);
}


void init_orders(struct OrderList *order_list){
    for(int i = 0; i < 4; i++){
        order_list->orders[i] = -1;
    }
    order_list->last_order_idx = -1;
}

int main(){
    struct OrderList up_orders;
    struct OrderList dwn_orders;

    init_orders(&up_orders);
    init_orders(&dwn_orders);

    int motor_dir = DIRN_STOP;

    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){
        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }
        
        if (motor_dir = DIRN_STOP) {
            if (floor > 1) {
                if (up_orders.last_order_idx != -1) {
                    motor_dir = DIRN_UP;
                }
            }
        }

        if (motor_dir == DIRN_UP) {
            if (is_in_list(up_orders.orders,floor, 4)) {
                complete_order(&up_orders, false);
            } if (up_orders.last_order_idx == -1) {
                motor_dir = DIRN_STOP;
            } 
        }

        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                switch (b)
                {
                case 0:
                    add_order(&up_orders,f);
                    break;

                case 1:
                    add_order(&dwn_orders,f);
                    break;

                case 2:
                    if(!(floor == f && motor_dir == DIRN_STOP)){
                        if(floor < f){
                            add_order(&up_orders,f);
                        }else if (floor > f){
                            add_order(&dwn_orders,f);
                        }
                    }
                    break;
                
                default:
                    break;
                }

                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            motor_dir = DIRN_STOP;
            break;
        }
        
        //nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);

        elevio_motorDirection(motor_dir);
    }

    return 0;
    
}
