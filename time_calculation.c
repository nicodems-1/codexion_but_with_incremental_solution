#include "include.h"

int    time_initialization(t_param *param)
{
    int verification;
    struct timeval tv;
    verification = gettimeofday(&tv, NULL);
    param->time_since_beginning = (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
    return verification;
}

int current_time(t_param *param)
{
    int verification;
    unsigned long actual; 
    struct timeval tv;

    verification = gettimeofday(&tv, NULL);
    actual = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000));

    param->time_elapsed = actual - param->time_since_beginning;
    return (verification);
}