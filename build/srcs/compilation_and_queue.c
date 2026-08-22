/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation_and_queue.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:24 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 05:14:56 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int	debug(t_coder *coder)
{
	print_logs("is debugging", coder);
	if (ft_usleep(coder->param->time_to_debug, coder) != 0)
		return (1);
	return (0);
}

int	refactor(t_coder *coder)
{
	print_logs("is refactoring", coder);
	if (ft_usleep(coder->param->time_to_refactor, coder) != 0)
		return (1);
	return (0);
}

void	edf_queue(t_dongle *dongle, t_coder *coder)
{
	unsigned long	deadline;

	deadline = coder->last_compiled + coder->param->time_to_burnout;
	if (dongle->dongle_queue[0].coder_id == -1)
	{
		dongle->dongle_queue[0].coder_id = coder->id;
		dongle->dongle_queue[0].deadline = deadline;
	}
	else if ((dongle->dongle_queue[0].coder_id != -1)
		&& (dongle->dongle_queue[0].deadline > deadline)
		&& (dongle->dongle_queue[0].is_compiling == 0))
	{
		dongle->dongle_queue[1].coder_id = dongle->dongle_queue[0].coder_id;
		dongle->dongle_queue[1].deadline = dongle->dongle_queue[0].deadline;
		dongle->dongle_queue[1].is_compiling = 0;
		dongle->dongle_queue[0].deadline = deadline;
		dongle->dongle_queue[0].coder_id = coder->id;
		dongle->dongle_queue[0].is_compiling = 0;
	}
	else
	{
		dongle->dongle_queue[1].coder_id = coder->id;
		dongle->dongle_queue[1].deadline = deadline;
	}
}

void	update_dongle_queue(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	if (coder->last_compiled == 0)
		coder->last_compiled = current_time(coder->param);
	pthread_mutex_unlock(&coder->coder_mutex);
	pthread_mutex_lock(&dongle->dongle_lock);
	if (strcmp("fifo", coder->param->scheduler) == 0)
	{
		if (dongle->dongle_queue[0].coder_id == -1)
			dongle->dongle_queue[0].coder_id = coder->id;
		else
			dongle->dongle_queue[1].coder_id = coder->id;
	}
	if (strcmp("edf", coder->param->scheduler) == 0)
		edf_queue(dongle, coder);
	pthread_mutex_unlock(&dongle->dongle_lock);
}

int	compilation(t_coder *coder)
{
	if (get_dongles(coder) == 1)
		return (1);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->times_compiled += 1;
	coder->last_compiled = current_time(coder->param);
	pthread_mutex_unlock(&coder->coder_mutex);
	print_logs("is compiling", coder);
	if (ft_usleep(coder->param->time_to_compile, coder) == 1)
		return (1);
	release_dongles(coder);
	return (0);
}
