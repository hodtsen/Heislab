#include <time.h>
#include "timer_utils.h"

void timer_sleep_ms(long duration_ms) {
    nanosleep(&(struct timespec){duration_ms / 1000, (duration_ms % 1000) * 1000000L}, NULL);
}
