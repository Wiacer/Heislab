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

void complete_order() {
    int a;
}