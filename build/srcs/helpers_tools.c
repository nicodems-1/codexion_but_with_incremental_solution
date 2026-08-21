/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:47 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/21 07:49:21 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

unsigned long	current_time(t_param *param)
{
	unsigned long	current;
	struct timeval	tv;
	
	pthread_mutex_lock(&param->time_mutex);
	gettimeofday(&tv, NULL);
	if (param->time_initialized == 0)
		param->time_initialized = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000));
	current = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000))
		- param->time_initialized;
	pthread_mutex_unlock(&param->time_mutex);
	return (current);
}

int	print_logs(char *action, t_coder *coder)
{
	pthread_mutex_lock(&coder->param->print_lock);
	pthread_mutex_lock(&coder->param->update_status);
	if (coder->param->status == RUNNING)
	{
		printf("%ld %d %s\n", current_time(coder->param), coder->id, action);
		pthread_mutex_unlock(&coder->param->update_status);
		pthread_mutex_unlock(&coder->param->print_lock);
	}
	else
	{
		pthread_mutex_unlock(&coder->param->update_status);
		pthread_mutex_unlock(&coder->param->print_lock);
	}
	return (0);
}

int	is_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->param->update_status);
	if (coder->param->status == BURNOUT || coder->param->status == FINISHED)
	{
		pthread_mutex_unlock(&coder->param->update_status);
		return (1);
	}
	pthread_mutex_unlock(&coder->param->update_status);
	return (0);
}

int	ft_usleep(unsigned long time_to_sleep, t_coder *coder)
{
	unsigned long	begin_time;
	unsigned long	end_time;

	begin_time = current_time(coder->param);
	end_time = begin_time + time_to_sleep;
	while (current_time(coder->param) < end_time)
	{
		if (is_burnout(coder) == 1)
			return (1);
		usleep(100);
	}
	return (0);
}

int	check_cooldown(t_coder *coder, t_dongle *dongle)
{
	unsigned long	cooling_check;

	if (dongle->init == 0)
	{
		dongle->init = 1;
		return (0);
	}
	cooling_check = current_time(coder->param) - dongle->released_time;
	if (cooling_check > coder->param->dongle_cooldown)
		return (0);
	else
		return (1);
}