#include "include.h"

unsigned long current_time()
{
    static unsigned long starting_time = 0;
    unsigned long current; 

    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (starting_time == 0)
        starting_time = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000));
    current = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000)) - starting_time;
    return(current);
}