#include "include.h"


int ft_usleep(unsigned long time_to_sleep)
{
    unsigned long begin_time = current_time();
    unsigned long end_time = begin_time + time_to_sleep;
    while(current_time() < end_time)
    {
        usleep(1);
    }
    return(0);
}
int debug(t_coder *coder)
{
    ft_usleep(coder->param->time_to_debug);
    print_logs("is debugging", coder);
    return(0);
}
int refactor(t_coder *coder)
{
    ft_usleep(coder->param->time_to_refactor);
    print_logs("is refactoring", coder);
    return(0);
}