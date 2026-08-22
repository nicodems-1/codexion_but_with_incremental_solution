/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 22:39:56 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 04:20:17 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	join_threads(t_coder *coder_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(coder_array[i].coder, NULL);
		i++;
	}
}

void	destroy_mutex_spe(t_param *param, int count)
{
	if (count >= 0)
		pthread_mutex_destroy(&param->update_status);
	if (count >= 1)
		pthread_mutex_destroy(&param->lock_race);
	if (count >= 2)
		pthread_cond_destroy(&param->starting_race);
	if (count >= 3)
		pthread_mutex_destroy(&param->print_lock);
	if (count >= 4)
		pthread_mutex_destroy(&param->time_mutex);
}

void	destroy_coder_mutexes(t_coder *coder_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&coder_array[i].coder_mutex);
		i++;
	}
}

void	destroy_dongles_mutexes(t_dongle *dongle_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&dongle_array[i].dongle_lock);
		i++;
	}
}

void	free_allocation(t_coder *coder_array, t_dongle *dongle_array)
{
	free(coder_array);
	free(dongle_array);
}
