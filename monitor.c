#include "include.h"

void *monitor(void *arguments)
{
    t_coder *coders;
    coders = arguments;
    int time_to_burnout = coders[0].param->time_to_burnout;
    int nb_of_coders = coders[0].param->number_of_coders;
    int i = 0;
    // pthread_mutex_lock(&coders[0].param->lock_race);
    // while(coders[0].param->unlock_race != 1)
	// 	pthread_cond_wait(&coders[0].param->starting_race, &coders[0].param->lock_race);
    // pthread_mutex_unlock(&coders[0].param->lock_race);

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