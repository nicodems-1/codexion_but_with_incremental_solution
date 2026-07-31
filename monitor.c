#include "include.h"

void monitor(t_coder *coders)
{
    int time_to_burnout = 450;
    int nb_of_coders = 2;
    int i = 0;
    int status;
    while(1)
    {
        usleep(50);
        while(i <= nb_of_coders)
        {
            pthread_mutex_lock(&coders[i].coder_mutex);
            if ((current_time() - coders[i].last_compiled) > time_to_burnout)
                status = BURNOUT;
            pthread_mutex_unlock(&coders[i].coder_mutex);
            i++;
        }
    }
}