#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
//#include "helpers.h"

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

int stateSpace[3], orderSpace[10], idx = 100;

void check_for_orders(){

    if(elevio_callButton(3, BUTTON_HALL_DOWN)){     // indeks 0 i orderSpace
        if(orderSpace[0] == 0){
            orderSpace[0] = idx;
            idx++;
            elevio_buttonLamp(3, BUTTON_HALL_DOWN,1);
        }
    }

    if(elevio_callButton(2, BUTTON_HALL_DOWN)){     // indeks 1 i orderSpace
        if(orderSpace[1] == 0){
            orderSpace[1] = idx;
            idx++;
            elevio_buttonLamp(2, BUTTON_HALL_DOWN,1);
        }
    }

    if(elevio_callButton(1, BUTTON_HALL_DOWN)){     // indeks 2 i orderSpace
        if(orderSpace[2] == 0){
            orderSpace[2] = idx;
            idx++;
            elevio_buttonLamp(1, BUTTON_HALL_DOWN,1);
        }
    }

    if(elevio_callButton(2, BUTTON_HALL_UP)){       // indeks 3 i orderSpace
        if(orderSpace[3] == 0){
            orderSpace[3] = idx;
            idx++;
            elevio_buttonLamp(2, BUTTON_HALL_UP,1);
        }
    }

    if(elevio_callButton(1, BUTTON_HALL_UP)){       // indeks 4 i orderSpace
        if(orderSpace[4] == 0){
            orderSpace[4] = idx;
            idx++;
            elevio_buttonLamp(1, BUTTON_HALL_UP,1);
        }
    }

    if(elevio_callButton(0, BUTTON_HALL_UP)){       // indeks 5 i orderSpace
        if(orderSpace[5] == 0){
            orderSpace[5] = idx;
            idx++;
            elevio_buttonLamp(0, BUTTON_HALL_UP,1);
        }
    }

    if(elevio_callButton(3, BUTTON_CAB)){           // indeks 6 i orderSpace
        if(orderSpace[6] == 0){
            orderSpace[6] = idx;
            idx++;
            elevio_buttonLamp(3, BUTTON_CAB, 1);
        }
    }

    if(elevio_callButton(2, BUTTON_CAB)){           // indeks 7 i orderSpace
        if(orderSpace[7] == 0){
            orderSpace[7] = idx;
            idx++;
            elevio_buttonLamp(2, BUTTON_CAB, 1);
        }
    }

    if(elevio_callButton(1, BUTTON_CAB)){           // indeks 8 i orderSpace
        if(orderSpace[8] == 0){
            orderSpace[8] = idx;
            idx++;
            elevio_buttonLamp(1, BUTTON_CAB, 1);
        }
    }

    if(elevio_callButton(0, BUTTON_CAB)){           // indeks 9 i orderSpace
        if(orderSpace[9] == 0){
            orderSpace[9] = idx;
            idx++;
            elevio_buttonLamp(0, BUTTON_CAB, 1);
        }
    }

    if(idx > 1000000000){
        for(int i = 0 ; i<10 ; ++i){
            if(orderSpace[i] > 0)orderSpace[i]-=999999900;
        }
        idx-=999999900;
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

void wait_3s(){
    int should_restart;
    restart_wait:
    should_restart = 0;
    for(int i = 0 ; i<120 ; ++i){
        check_for_orders();
        while(elevio_stopButton()){
            elevio_stopLamp(1);
            should_restart = 1;
        }
        if(should_restart == 1){
            elevio_stopLamp(0);
            goto restart_wait;
        }
        if(elevio_obstruction())goto restart_wait;
        nanosleep(&(struct timespec) {0,25*1000*1000}, NULL);
    }
    
}

int nr_of_orders(){
    int nr = 0;
    for(int i = 0 ; i<10 ; ++i)
        if(orderSpace[i] > 0) nr++;
    return nr;
}

void lights_off(){
    // buttons
    elevio_buttonLamp(3,BUTTON_HALL_DOWN,0);
    elevio_buttonLamp(2,BUTTON_HALL_DOWN,0);
    elevio_buttonLamp(1,BUTTON_HALL_DOWN,0);
    elevio_buttonLamp(2,BUTTON_HALL_UP,0);
    elevio_buttonLamp(1,BUTTON_HALL_UP,0);
    elevio_buttonLamp(0,BUTTON_HALL_UP,0);
    elevio_buttonLamp(3,BUTTON_CAB,0);
    elevio_buttonLamp(2,BUTTON_CAB,0);
    elevio_buttonLamp(1,BUTTON_CAB,0);
    elevio_buttonLamp(0,BUTTON_CAB,0);

    // floor 0
    elevio_floorIndicator(0);

    // door open
    elevio_doorOpenLamp(0);

    // stop
    elevio_stopLamp(0);
}

void update_lights(int lastfloor, int dir){
    elevio_buttonLamp(lastfloor, BUTTON_CAB, 0);
    orderSpace[9-lastfloor] = 0;
    if(dir == 1){
        elevio_buttonLamp(lastfloor, BUTTON_HALL_UP, 0);
        orderSpace[5-lastfloor] = 0;
    }
    if(dir == -1){
        elevio_buttonLamp(lastfloor, BUTTON_HALL_DOWN, 0);
        orderSpace[3-lastfloor] = 0;
    }
}

void reboot(){
    lights_off();
    elevio_motorDirection(DIRN_DOWN);
    while(elevio_floorSensor() != 0) {}
    elevio_motorDirection(DIRN_STOP);
}

void head_for_floor(int nextfloor, int lastfloor, int *dir){
    if(nextfloor == -1)return;
    *dir = (nextfloor < lastfloor ? -1 : 1);
    if(nextfloor == lastfloor) elevio_motorDirection(DIRN_STOP);
    else (*dir == 1 ? elevio_motorDirection(DIRN_UP) : elevio_motorDirection(DIRN_DOWN));
}

void check_for_stop(int *floor, int *dir, int *nextfloor, int *lastfloor){
    if(elevio_stopButton()){
        *nextfloor = *lastfloor;
        resetOrders();
        lights_off();
        elevio_motorDirection(DIRN_STOP);
        int on_floor = 0;
        elevio_stopLamp(1);
        while(elevio_stopButton()) {if(*floor > -1){elevio_doorOpenLamp(1);on_floor = 1;}}
        elevio_stopLamp(0);
        if(on_floor == 1){
            elevio_doorOpenLamp(1);
            wait_3s();
        }
        elevio_doorOpenLamp(0);
        reboot();                              // TEMPORARY, STOP BUTTON DOES NOT REALLY WORK
    }
}

int wait_for_orders(int *floor, int *dir, int *nextfloor, int *lastfloor){
    elevio_motorDirection(DIRN_STOP);
    while(nr_of_orders() == 0){
        check_for_orders();
        check_for_stop(floor, dir, nextfloor, lastfloor);
    }
    if(orderSpace[5] > 0 || orderSpace[9] > 0) return 0;
    if(orderSpace[2] > 0 || orderSpace[4] > 0 || orderSpace[8] > 0)return 1;
    if(orderSpace[1] > 0 || orderSpace[3] > 0 || orderSpace[7] > 0)return 2;
    if(orderSpace[0] > 0 || orderSpace[6] > 0)return 3;
    return -1;
}

void open_doors(int *floor, int *lastfloor, int *dir){
    elevio_motorDirection(DIRN_STOP);
    elevio_floorIndicator(elevio_floorSensor());
    elevio_doorOpenLamp(1);
    wait_3s();
    elevio_doorOpenLamp(0);
    if(*dir == 1) elevio_motorDirection(DIRN_UP);
    else elevio_motorDirection(DIRN_DOWN);
    *lastfloor = *floor;
}

int main(){
    //initial values
    for(int i = 0 ; i<10 ; ++i)orderSpace[i]=0;
    elevio_init();

    illegal_floor_reset:
    reboot();
    int cur_dir = -1;
    int lastfloor = 0;
    int nextfloor = 0;
    int floor = 0;

    while(1){
        nextfloor = wait_for_orders(&floor, &cur_dir, &nextfloor, &lastfloor);
        if(nextfloor == -1)goto illegal_floor_reset;

        head_for_floor(nextfloor, lastfloor, &cur_dir);
        
        while(floor != nextfloor){
            floor = elevio_floorSensor();
            check_for_orders();
            check_for_stop(&floor, &cur_dir, &nextfloor, &lastfloor);
            head_for_floor(nextfloor, lastfloor, &cur_dir);
            printf("%d %d %d\n",floor,lastfloor,nextfloor);
            printOrderSpace();

            if(floor > -1){
                lastfloor = floor;
                if(cur_dir == 1 && orderSpace[5-lastfloor] > 0){
                    open_doors(&floor, &lastfloor, &cur_dir);
                    update_lights(lastfloor, cur_dir);
                }
                if(cur_dir == -1 && orderSpace[3-lastfloor] > 0){
                    open_doors(&floor, &lastfloor, &cur_dir);
                    update_lights(lastfloor, cur_dir);
                }
                if(orderSpace[9-lastfloor] > 0){
                    open_doors(&floor, &lastfloor, &cur_dir);
                    update_lights(lastfloor, cur_dir);
                }
                elevio_floorIndicator(floor);
            }
        }

        open_doors(&floor, &lastfloor, &nextfloor);
        update_lights(lastfloor, cur_dir);

        if(floor == 0 || floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_STOP);
        }

        check_for_stop(&floor, &cur_dir, &nextfloor, &lastfloor);
    }

    return 0;
}
