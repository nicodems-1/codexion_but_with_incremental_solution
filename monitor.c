#include "include.h"

void *monitor(void *arguments)
{
    t_coder *coders;
    coders = arguments;
    int nb_of_finished;
    int i;
    int time_to_burnout = coders[0].param->time_to_burnout;
    int nb_of_coders = coders[0].param->number_of_coders;

    pthread_mutex_lock(&coders[0].param->lock_race);
    while(coders[0].param->unlock_race != 1)
		pthread_cond_wait(&coders[0].param->starting_race, &coders[0].param->lock_race);
    pthread_mutex_unlock(&coders[0].param->lock_race);

    while(1)
    {
        i = 0;
        nb_of_finished = 0;
        while(i < nb_of_coders)
        {
            pthread_mutex_lock(&coders[i].coder_mutex);
            if ((current_time() - (unsigned long) coders[i].last_compiled) > (unsigned long) time_to_burnout)
            {
                print_logs("has burnout", &coders[i]);
                pthread_mutex_lock(&coders[0].param->update_status);
                coders[0].param->status = BURNOUT;
                pthread_mutex_unlock(&coders[0].param->update_status);
                pthread_mutex_unlock(&coders[i].coder_mutex);
                return NULL;
            }
            if(coders[i].times_compiled > coders[i].param->number_of_compiles_required)
            {
                nb_of_finished++;
            }
            if(nb_of_finished == nb_of_coders)
            {
                pthread_mutex_lock(&coders[0].param->update_status);
                printf("FINISHED\n");
                coders[0].param->status = FINISHED;
                pthread_mutex_unlock(&coders[0].param->update_status);
                pthread_mutex_unlock(&coders[i].coder_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&coders[i].coder_mutex);
            i++;
        }
        usleep(5);
    }
    return(NULL);
}
