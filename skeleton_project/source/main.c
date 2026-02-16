#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevatorutils.h"

int main(){

elevio_init();
int motor_dir = DIRN_STOP;

ElevState state = MV_IDLE;

int orders[4][2] = {{0,0},{0,0},{0,0},{0,0}};
elevio_motorDirection(DIRN_DOWN);
while (elevio_floorSensor() == -1);
printf("Initialization done\n");
elevio_motorDirection(DIRN_STOP);
printf("=== Example Program ===\n");
printf("Press the stop button on the elevator panel to exit\n");

while(1){

int floor = elevio_floorSensor();

printf("1");
for(int f = 0; f < N_FLOORS; f++){
for(int b = 0; b < N_BUTTONS; b++){
int btnPressed = elevio_callButton(f, b);
elevio_buttonLamp(f, b, btnPressed);
if (btnPressed) {
if (f == floor && motor_dir != DIRN_STOP) {
if (motor_dir == DIRN_UP) {
orders[f][1] = 1;
} else if (motor_dir == DIRN_DOWN) {
orders[f][0] = 1;
}
} else if (f != floor) {
if (b != 2) {
orders[f][b] = 1;
} else {
if (floor < f) {
orders[f][0] = 1;
} else {
orders[f][1] = 1;
}
}
}
}
}
}

switch (state) {
case MV_DWN:
break;
case MV_IDLE:
break;
case MV_UP:
break;
case SRVC:
break;
}
/*



printf("%d\n",floor);

if (floor != -1){
if (motor_dir == DIRN_UP) {
motor_dir = DIRN_STOP;
if (orders[floor][0] == 1) {
elevio_motorDirection(motor_dir);
complete_order();
orders[floor][0] = 0;
}
for (int i = floor+1; i < 4; i++) {
if (orders[i][0] == 1 || orders[i][1] == 1) {
motor_dir = DIRN_UP;
elevio_motorDirection(motor_dir);
}
}

} else if (motor_dir == DIRN_DOWN) {
motor_dir = DIRN_STOP;
if (orders[floor][1] == 1) {
elevio_motorDirection(motor_dir);
complete_order();
orders[floor][1] = 0;
}
for (int i = 0; i < floor; i++) {
if (orders[i][0] == 1 || orders[i][1] == 1) {
motor_dir = DIRN_DOWN;
elevio_motorDirection(motor_dir);
}
}
} else if (motor_dir == DIRN_STOP) {
for (int i = 0; i < 4; i++) {
for (int j = 0; j < 2; j++) {
if (orders[i][j] == 1) {
if (i < floor) {
motor_dir = DIRN_DOWN;
} else {
motor_dir = elevio_motorDirectionDIRN_UP;
}
}
}
}
}
}

elevio_motorDirection(motor_dir);

for(int f = 0; f < N_FLOORS; f++){
for(int b = 0; b < N_BUTTONS; b++){
int btnPressed = elevio_callButton(f, b);
elevio_buttonLamp(f, b, btnPressed);
if (btnPressed) {
if (f == floor && motor_dir != DIRN_STOP) {
if (motor_dir == DIRN_UP) {
orders[f][1] = 1;
} else if (motor_dir == DIRN_DOWN) {
orders[f][0] = 1;
}
} else if (f != floor) {
if (b != 2) {
orders[f][b] = 1;
} else {
if (floor < f) {
orders[f][0] = 1;
} else {
orders[f][1] = 1;
}
}
}
}
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
// nanosleep(&(struct timespec){0, 20*1000*1000}, NULL); */
//elevio_motorDirection(DIRN_DOWN);
}

return 0;
}