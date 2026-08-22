/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:57:16 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 02:59:07 by niverdie         ###   ########.fr       */
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
	if (coder->param->number_of_coders == 1)
	{
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		print_logs("has taken a dongle", coder);
		pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
		ft_usleep(coder->param->time_to_burnout, coder);
		return (NULL);
	}
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

	if (alloc_memory(param) != 0)
		return (clean_exit(param, 1, 0));
	dongle = param->dongles;
	coder = param->coders;
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
