/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation_coders.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:24 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 02:25:31 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

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

int	wait_for_dongle(t_dongle *dongle, t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&dongle->dongle_lock);
		if (dongle->dongle_queue[0].coder_id == coder->id
			&& check_cooldown(coder, dongle) == 0)
		{
			pthread_mutex_unlock(&dongle->dongle_lock);
			return (0);
		}
		pthread_mutex_unlock(&dongle->dongle_lock);
		if (is_burnout(coder) == 1)
			return (1);
		usleep(100);
	}
	return (0);
}

int	update_dongle_queue(t_dongle *dongle, t_coder *coder)
{
	unsigned long	deadline;

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
	{
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
	pthread_mutex_unlock(&dongle->dongle_lock);
	return (0);
}

int	get_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		update_dongle_queue(coder->left_dongle, coder);
		if (wait_for_dongle(coder->left_dongle, coder) == 1)
			return (1);
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		coder->left_dongle->dongle_queue[0].is_compiling = 1;
		pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
		print_logs("has taken a dongle", coder);
		update_dongle_queue(coder->right_dongle, coder);
		if (wait_for_dongle(coder->right_dongle, coder) == 1)
			return (1);
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		coder->right_dongle->dongle_queue[0].is_compiling = 1;
		pthread_mutex_unlock(&coder->right_dongle->dongle_lock);
		print_logs("has taken a dongle", coder);
	}
	else
	{
		update_dongle_queue(coder->right_dongle, coder);
		if (wait_for_dongle(coder->right_dongle, coder) == 1)
			return (1);
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		coder->right_dongle->dongle_queue[0].is_compiling = 1;
		pthread_mutex_unlock(&coder->right_dongle->dongle_lock);
		print_logs("has taken a dongle", coder);
		update_dongle_queue(coder->left_dongle, coder);
		if (wait_for_dongle(coder->left_dongle, coder) == 1)
			return (1);
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		coder->left_dongle->dongle_queue[0].is_compiling = 1;
		pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
		print_logs("has taken a dongle", coder);
	}
	return (0);
}

int	release_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->dongle_lock);
	coder->left_dongle->dongle_queue[0].coder_id = coder->left_dongle->dongle_queue[1].coder_id;
	coder->left_dongle->dongle_queue[1].coder_id = -1;
	coder->left_dongle->released_time = current_time(coder->param);
	coder->left_dongle->dongle_queue[0].is_compiling = 0;
	coder->left_dongle->dongle_queue[0].deadline = coder->left_dongle->dongle_queue[1].deadline;
	pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
	pthread_mutex_lock(&coder->right_dongle->dongle_lock);
	coder->right_dongle->dongle_queue[0].coder_id = coder->right_dongle->dongle_queue[1].coder_id;
	coder->right_dongle->dongle_queue[1].coder_id = -1;
	coder->right_dongle->released_time = current_time(coder->param);
	coder->right_dongle->dongle_queue[0].is_compiling = 0;
	coder->right_dongle->dongle_queue[0].deadline = coder->right_dongle->dongle_queue[1].deadline;
	pthread_mutex_unlock(&coder->right_dongle->dongle_lock);
	return (0);
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
