#include "include.h"

void	*monitor(void *arguments)
{
	t_coder	*coders;
	int		nb_of_finished;
	int		i;
	int		time_to_burnout;
	int		nb_of_coders;

	coders = arguments;
	time_to_burnout = coders[0].param->time_to_burnout;
	nb_of_coders = coders[0].param->number_of_coders;
	pthread_mutex_lock(&coders[0].param->lock_race);
	while (coders[0].param->unlock_race != 1)
		pthread_cond_wait(&coders[0].param->starting_race,
			&coders[0].param->lock_race);
	pthread_mutex_unlock(&coders[0].param->lock_race);
	while (1)
	{
		i = 0;
		nb_of_finished = 0;
		while (i < nb_of_coders)
		{
			pthread_mutex_lock(&coders[i].coder_mutex);
			if ((current_time(coders[0].param)
					- (unsigned long)coders[i].last_compiled) > (unsigned long)time_to_burnout)
			{
				pthread_mutex_lock(&coders->param->print_lock);
				pthread_mutex_lock(&coders[0].param->update_status);
				coders[0].param->status = BURNOUT;
				printf("%lu %d has burnout",current_time(coders[0].param), i);
				pthread_mutex_unlock(&coders->param->print_lock);
				pthread_mutex_unlock(&coders[0].param->update_status);
				pthread_mutex_unlock(&coders[i].coder_mutex);
				return (NULL);
			}
			if (coders[i].times_compiled > coders[i].param->number_of_compiles_required)
				nb_of_finished++;
			if (nb_of_finished == nb_of_coders)
			{
				pthread_mutex_lock(&coders[0].param->print_lock);
				pthread_mutex_lock(&coders[0].param->update_status);
				coders[0].param->status = FINISHED;
				printf("FINISHED\n");
				pthread_mutex_unlock(&coders[0].param->update_status);
				pthread_mutex_unlock(&coders[i].coder_mutex);
				pthread_mutex_unlock(&coders->param->print_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&coders[i].coder_mutex);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
