/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:24 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/19 17:49:32 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	ft_usleep(unsigned long time_to_sleep, t_coder *coder)
{
	unsigned long	begin_time;
	unsigned long	end_time;

	begin_time = current_time(coder->param);
	end_time = begin_time + time_to_sleep;
	while (current_time(coder->param) < end_time)
	{
		usleep(100);
		pthread_mutex_lock(&coder->param->update_status);
		if (coder->param->status == BURNOUT || coder->param->status == FINISHED)
		{
			pthread_mutex_unlock(&coder->param->update_status);
			return (1);
		}
		pthread_mutex_unlock(&coder->param->update_status);
	}
	return (0);
}

int	debug(t_coder *coder)
{
	print_logs("is debugging", coder);
	if (ft_usleep(coder->param->time_to_debug, coder) != 0)
		return (1);
	return (0);
}

int	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->param->update_status);
	if (coder->param->status == BURNOUT)
		print_logs("has burnout", coder);
	pthread_mutex_unlock(&coder->param->update_status);
	print_logs("is refactoring", coder);
	if (ft_usleep(coder->param->time_to_refactor, coder) != 0)
		return (1);
	return (0);
}

int wait_for_dongle(t_dongle *dongle, t_coder *coder)
{
	while(1)
	{
		pthread_mutex_lock(&dongle->dongle_lock);
		if(dongle->dongle_queue[0] == coder->id)
		{
			pthread_mutex_unlock(&dongle->dongle_lock);
			break;
		}
		pthread_mutex_unlock(&dongle->dongle_lock);
		usleep(100);
	}
	return(0);
}
int	update_dongle_queue(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->dongle_lock);
	if(dongle->dongle_queue[0] == -1)
		dongle->dongle_queue[0] = coder->id;
	else
		dongle->dongle_queue[1] = coder->id;
	pthread_mutex_unlock(&dongle->dongle_lock);
	return (0);
}
int	get_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		update_dongle_queue(coder->left_dongle, coder);
		wait_for_dongle(coder->left_dongle, coder);
		print_logs("has taken a dongle", coder);
		update_dongle_queue(coder->right_dongle, coder);
		wait_for_dongle(coder->right_dongle, coder);
		print_logs("has taken a dongle", coder);
	}
	else
	{
		update_dongle_queue(coder->right_dongle, coder);
		wait_for_dongle(coder->right_dongle, coder);
		print_logs("has taken a dongle", coder);
		update_dongle_queue(coder->left_dongle, coder);
		wait_for_dongle(coder->left_dongle, coder);
		print_logs("has taken a dongle", coder);
	}
	return (0);
}
int 	release_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->dongle_lock);
	coder->left_dongle->dongle_queue[0] = coder->left_dongle->dongle_queue[1];
	coder->left_dongle->dongle_queue[1] = -1;
	coder->left_dongle->released_time = current_time(coder->param);
	pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
	pthread_mutex_lock(&coder->right_dongle->dongle_lock);
	coder->right_dongle->dongle_queue[0] = coder->right_dongle->dongle_queue[1];
	coder->right_dongle->dongle_queue[1] = -1;
	coder->right_dongle->released_time = current_time(coder->param);
	pthread_mutex_unlock(&coder->right_dongle->dongle_lock);
	return(0);
}

int	compilation(t_coder *coder)
{
	get_dongles(coder);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->times_compiled += 1;
	coder->last_compiled = current_time(coder->param);
	pthread_mutex_unlock(&coder->coder_mutex);
	print_logs("is compiling", coder);
	release_dongles(coder);
	ft_usleep(coder->param->time_to_compile, coder);
	return (0);
}
