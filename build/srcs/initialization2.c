/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 22:42:42 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 04:20:30 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

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
			return (clean_exit(param, 3, index));
		dongle[index].released_time = 0;
		dongle[index].dongle_queue[0].coder_id = -1;
		dongle[index].dongle_queue[1].coder_id = -1;
		dongle[index].dongle_queue->is_compiling = 0;
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
			return (clean_exit(param, 2, index));
		index++;
	}
	return (0);
}

int	alloc_memory(t_param *param)
{
	param->coders = NULL;
	param->dongles = NULL;
	param->coders = malloc(param->number_of_coders * sizeof(t_coder));
	param->dongles = malloc(param->number_of_coders * sizeof(t_dongle));
	if (!param->dongles || !param->coders)
		return (1);
	return (0);
}
