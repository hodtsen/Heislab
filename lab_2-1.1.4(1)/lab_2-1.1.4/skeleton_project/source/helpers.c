#include "helpers.h"

void wait_three_seconds(){

    
    for(int i = 0 ; i<30 ; ++i)
        nanosleep(&(struct timespec) {0,100*1000*1000}, NULL);

    
}

void check_for_updates(){

}