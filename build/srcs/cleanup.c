/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:29 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/21 22:13:56 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

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
static int	get_count(int current_step, int failed_step, int failed_idx,
		int total)
{
	if (current_step == failed_step)
		return (failed_idx);
	return (total);
}

int	clean_exit(t_param *param, int step, int failed_idx)
{
	t_coder		*coder;
	t_dongle	*dongle;
	int			total;

	coder = NULL;
	dongle = param->dongles;
	total = param->number_of_coders;
	if (step >= 6)
		pthread_join(param->monitor_thread, NULL);
	if (step >= 5)
		join_threads(coder, get_count(5, step, failed_idx, total));
	if (step >= 4)
		destroy_mutex_spe(param, get_count(5, step, failed_idx, total));
	if (step >= 3)
		destroy_dongles_mutexes(dongle, get_count(3, step, failed_idx, total));
	if (step >= 2)
		destroy_coder_mutexes(coder, get_count(2, step, failed_idx, total));
	if (step >= 1)
		free_allocation(coder, dongle);
	if (step <= 6)
		return (1);
	else
		return (0);
}
