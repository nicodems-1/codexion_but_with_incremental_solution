/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:29 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/15 22:39:05 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

void	join_threads(t_coder *coder_array)
{
	t_param	*param;
	int		i;

	param = coder_array[0].param;
	i = 0;
	pthread_join(param->monitor_thread, NULL);
	while (i < param->number_of_coders)
	{
		pthread_join(coder_array[i].coder, NULL);
		i++;
	}
}

void	destroy_mutexes(t_coder *coder_array, t_dongle *dongle_array)
{
	t_param	*param;
	int		i;

	param = coder_array[0].param;
	pthread_mutex_destroy(&param->update_status);
	pthread_mutex_destroy(&param->status_lock);
	pthread_mutex_destroy(&param->lock_race);
	pthread_cond_destroy(&param->starting_race);
	pthread_mutex_destroy(&param->print_lock);
	i = 0;
	while (i < param->number_of_coders)
	{
		pthread_mutex_destroy(&coder_array[i].coder_mutex);
		pthread_mutex_destroy(&dongle_array[i].dongle_lock);
		i++;
	}
}
void	free_allocation(t_coder *coder_array, t_dongle *dongle_array)
{
	t_param	*param;

	param = coder_array[0].param;
	free(coder_array);
	free(dongle_array);
}
void	clean_exit(t_coder *coder_array, t_dongle *dongle_array)
{
	join_threads(coder_array);
	destroy_mutexes(coder_array, dongle_array);
	free_allocation(coder_array, dongle_array);
}