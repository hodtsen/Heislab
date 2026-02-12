#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "helpers.h"

/* orderSpace saved button presses

0: Trykket "Ned" i 4. etasje
1:  ----- || ----- 3. etasje
2:  ----- || ----- 2. etasje

3: Trykket "Opp" i 3. etasje
4:  ----- || ----- 2. etasje
5:  ----- || ----- 1. etasje

6: Kalt på heisen i 4. etasje
7: ------ || ------ 3. etasje
8: ------ || ------ 2. etasje
9: ------ || ------ 1. etasje

*/

/* stateSpace
0: current floor
1: last floor (same as current floor if current floor > -1)
2: current durection
*/

int stateSpace[3], orderSpace[10];
int orders = 0;

void check_for_orders(){

    if(elevio_callButton(3, BUTTON_HALL_DOWN)){     // indeks 0 i orderSpace
        if(orderSpace[0] == 0){
            orderSpace[0] = 1;
            orders++;
        }
    }

    if(elevio_callButton(2, BUTTON_HALL_DOWN)){     // indeks 1 i orderSpace
        if(orderSpace[1] == 0){
            orderSpace[1] = 1;
            orders++;
        }
    }

    if(elevio_callButton(1, BUTTON_HALL_DOWN)){     // indeks 2 i orderSpace
        if(orderSpace[2] == 0){
            orderSpace[2] = 1;
            orders++;
        }
    }

    if(elevio_callButton(2, BUTTON_HALL_UP)){       // indeks 3 i orderSpace
        if(orderSpace[3] == 0){
            orderSpace[3] = 1;
            orders++;
        }
    }

    if(elevio_callButton(1, BUTTON_HALL_UP)){       // indeks 4 i orderSpace
        if(orderSpace[4] == 0){
            orderSpace[4] = 1;
            orders++;
        }
    }

    if(elevio_callButton(0, BUTTON_HALL_UP)){       // indeks 5 i orderSpace
        if(orderSpace[5] == 0){
            orderSpace[5] = 1;
            orders++;
        }
    }

    if(elevio_callButton(3, BUTTON_CAB)){           // indeks 6 i orderSpace
        if(orderSpace[6] == 0){
            orderSpace[6] = 1;
            orders++;
        }
    }

    if(elevio_callButton(2, BUTTON_CAB)){           // indeks 7 i orderSpace
        if(orderSpace[7] == 0){
            orderSpace[7] = 1;
            orders++;
        }
    }

    if(elevio_callButton(1, BUTTON_CAB)){           // indeks 8 i orderSpace
        if(orderSpace[8] == 0){
            orderSpace[8] = 1;
            orders++;
        }
    }

    if(elevio_callButton(0, BUTTON_CAB)){           // indeks 9 i orderSpace
        if(orderSpace[9] == 0){
            orderSpace[9] = 1;
            orders++;
        }
    }
}

void resetOrders(){
    for(int i = 0 ; i<10 ; ++i)orderSpace[i] = 0;
}

void printOrderSpace(){
    for(int i = 0 ; i<10 ; ++i){
        printf("%d ",orderSpace[i]);
    }
    printf("\n");
}

/*  WORK IN PROGRESS
void head_for_floor(int newFloor, int currentFloor){
    if(newFloor == currentFloor)return;
    newFloor > currentFloor ? elevio_motorDirection(DIRN_UP) : elevio_motorDirection(DIRN_DOWN);
}
//*/

int main(){
    //initial values
    for(int i = 0 ; i<10 ; ++i)orderSpace[i]=0;
    orders = 0;
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_DOWN);
    int cur_dir = -1;

    /*
    while(elevio_floorSensor( )!=0) {}

    elevio_motorDirection(DIRN_STOP);
    //*/

    while(1){
        printf("%d\n",elevio_floorSensor());

        int floor = elevio_floorSensor();
        int lastfloor = -1;

        check_for_orders();
        printOrderSpace();

        if(floor > -1){
            elevio_motorDirection(DIRN_STOP);
            elevio_doorOpenLamp(1);
            elevio_floorIndicator(elevio_floorSensor());
            wait_three_seconds();                                           //PERFECT
            elevio_doorOpenLamp(0);
            if(cur_dir == -1) elevio_motorDirection(DIRN_DOWN);
            else elevio_motorDirection(DIRN_UP);

            lastfloor = floor;
        }

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
            cur_dir = 1;
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
            cur_dir = -1;
        }


        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }

        if(elevio_stopButton()){
            resetOrders();
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);
            while (elevio_stopButton()) {}
            while (!elevio_stopButton()) {}
            elevio_stopLamp(0);
            if(cur_dir == 1) elevio_motorDirection(DIRN_UP);
            else elevio_motorDirection(DIRN_DOWN);
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
