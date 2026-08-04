#include "include.h"

void *monitor(void *arguments)
{
    t_coder *coders;
    coders = arguments;
    int time_to_burnout = coders[0].param->time_to_burnout;
    int nb_of_coders = coders[0].param->number_of_coders;
    int i = 0;
    while(1)
    {
        while(i < nb_of_coders)
        {
            pthread_mutex_lock(&coders[i].coder_mutex);
            if ((current_time() - (unsigned long) coders[i].last_compiled) > (unsigned long) time_to_burnout)
                coders[0].param->status = BURNOUT;
            pthread_mutex_unlock(&coders[i].coder_mutex);
            i++;
        }
        usleep(50);
    }
    return(NULL);
}