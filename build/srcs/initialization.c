/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:57:16 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/21 21:05:48 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

void	*routine(void *arguments)
{
	t_coder	*coder;

	coder = (t_coder *)arguments;
	pthread_mutex_lock(&coder->param->lock_race);
	while (coder->param->unlock_race != 1)
		pthread_cond_wait(&coder->param->starting_race,
			&coder->param->lock_race);
	pthread_mutex_unlock(&coder->param->lock_race);
	while (1)
	{
		if (compilation(coder) != 0)
			return (NULL);
		if (debug(coder) != 0)
			return (NULL);
		if (refactor(coder) != 0)
			return (NULL);
	}
	return (NULL);
}
int	mutex_init(t_param *param)
{
	int	error;

	error = 0;
	if (pthread_mutex_init(&param->print_lock, NULL) != 0)
		clean_exit(param, 4, 0);
	if (pthread_mutex_init(&param->lock_race, NULL))
		clean_exit(param, 4, 1);
	if (pthread_mutex_init(&param->update_status, NULL) != 0)
		clean_exit(param, 4, 2);
	if (pthread_mutex_init(&param->time_mutex, NULL) != 0)
		clean_exit(param, 4, 3);
	if (pthread_cond_init(&param->starting_race, NULL) != 0)
		clean_exit(param, 4, 4);
	return (error);
}

int	init_dongles(t_param *param, t_dongle *dongle)
{
	int	index;

	param->unlock_race = 0;
	index = 0;
	while (index < (param->number_of_coders))
	{
		if (pthread_mutex_init(&dongle[index].dongle_lock, NULL) != 0)
			return (clean_exit(param, 3, index + 1));
		dongle[index].released_time = 0;
		dongle[index].dongle_queue[0].coder_id = -1;
		dongle[index].dongle_queue[1].coder_id = -1;
		dongle[index].dongle_queue->is_eating = 0;
		dongle[index].dongle_queue->deadline = 0;
		dongle[index].init = 0;
		index++;
	}
	return (0);
}
int	init_coders_mutex(t_param *param, t_coder *coder)
{
	int	index;

	index = 0;
	while (index < (param->number_of_coders))
	{
		if (pthread_mutex_init(&coder[index].coder_mutex, NULL) != 0)
			return (clean_exit(param, 2, index + 1));
		index++;
	}
	return (0);
}
int	init_coders(t_param *param, t_coder *coder, t_dongle *dongle)
{
	int	index;

	index = 0;
	param->status = RUNNING;
	while (index < (param->number_of_coders))
	{
		coder[index].left_dongle = &dongle[index];
		coder[index].right_dongle = &dongle[(index + 1)
			% param->number_of_coders];
		coder[index].param = param;
		coder[index].id = index + 1;
		coder[index].times_compiled = 0;
		coder[index].last_compiled = 0;
		if (pthread_create(&coder[index].coder, NULL, &routine,
				&coder[index]) != 0)
			return (clean_exit(param, 5, index + 1));
		index++;
	}
	return (0);
}

int	initialization(t_param *param)
{
	t_coder		*coder;
	t_dongle	*dongle;
	pthread_t	monitor_thread;

	dongle = malloc(param->number_of_coders * sizeof(t_dongle));
	coder = malloc(param->number_of_coders * sizeof(t_coder));
	param->dongles = dongle;
	param->coders = coder;
	if (init_coders_mutex(param, coder) != 0)
		return (1);
	if (init_dongles(param, dongle) != 0)
		return (1);
	if (mutex_init(param) != 0)
		return (1);
	if (init_coders(param, coder, dongle) != 0)
		return (1);
	if (pthread_create(&monitor_thread, NULL, &monitor, coder) != 0)
		return (clean_exit(param, 6, 0));
	param->monitor_thread = monitor_thread;
	pthread_mutex_lock(&param->lock_race);
	param->unlock_race = 1;
	pthread_cond_broadcast(&param->starting_race);
	pthread_mutex_unlock(&param->lock_race);
	return (clean_exit(param, 7, param->number_of_coders));
}
