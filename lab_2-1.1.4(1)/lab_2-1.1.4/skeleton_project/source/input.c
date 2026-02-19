/*#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>


void input_floor_4D() {
    if(elevio_callButton(3, BUTTON_HALL_DOWN)){     // indeks 0 i orderSpace
        if(orderSpace[0] == 0){
            orderSpace[0] = 1;
            orders++;
        }
    }
}

void input_floor_3D() {
     if(elevio_callButton(2, BUTTON_HALL_DOWN)){     // indeks 1 i orderSpace
        if(orderSpace[1] == 0){
            orderSpace[1] = 1;
            orders++;
        }
    }
}

void input_floor_2D() {
    if(elevio_callButton(1, BUTTON_HALL_DOWN)){     // indeks 2 i orderSpace
        if(orderSpace[2] == 0){
            orderSpace[2] = 1;
            orders++;
        }
    }
}

void input_floor_1U() {
    if(elevio_callButton(2, BUTTON_HALL_UP)){       // indeks 3 i orderSpace
        if(orderSpace[3] == 0){
            orderSpace[3] = 1;
            orders++;
        }
    }
}

void input_floor_2U() {
    if(elevio_callButton(1, BUTTON_HALL_UP)){       // indeks 4 i orderSpace
        if(orderSpace[4] == 0){
            orderSpace[4] = 1;
            orders++;
        }
    }
}

void input_floor_3U() {
    if(elevio_callButton(0, BUTTON_HALL_UP)){       // indeks 5 i orderSpace
        if(orderSpace[5] == 0){
            orderSpace[5] = 1;
            orders++;
        }
    }
}

void input_cab_4() {
    if(elevio_callButton(3, BUTTON_CAB)){           // indeks 6 i orderSpace
        if(orderSpace[6] == 0){
            orderSpace[6] = 1;
            orders++;
        }
    }
}

void input_cab_3() {
    if(elevio_callButton(2, BUTTON_CAB)){           // indeks 7 i orderSpace
        if(orderSpace[7] == 0){
            orderSpace[7] = 1;
            orders++;
        }
    }

}

void input_cab_2() {
    if(elevio_callButton(1, BUTTON_CAB)){           // indeks 8 i orderSpace
        if(orderSpace[8] == 0){
            orderSpace[8] = 1;
            orders++;
        }
    }
}

void input_cab_1() {
    if(elevio_callButton(0, BUTTON_CAB)){           // indeks 9 i orderSpace
        if(orderSpace[9] == 0){
            orderSpace[9] = 1;
            orders++;
        }
    }
}
*/