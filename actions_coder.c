/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:24 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/11 12:37:51 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	ft_usleep(unsigned long time_to_sleep, t_coder *coder)
{
	unsigned long	begin_time;
	unsigned long	end_time;

	begin_time = current_time();
	end_time = begin_time + time_to_sleep;
	while (current_time() < end_time)
	{
		usleep(10);
		pthread_mutex_lock(&coder->param->update_status);
        if(coder->param->status == BURNOUT || coder->param->status == FINISHED)
		{
			pthread_mutex_unlock(&coder->param->update_status);
			return(1);
		}
        pthread_mutex_unlock(&coder->param->update_status);
	}
	return (0);
}

int	debug(t_coder *coder)
{
	print_logs("is debugging", coder);
	if(ft_usleep(coder->param->time_to_debug, coder) != 0)
		return(1);
	return (0);
}

int	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->param->status_lock);
	if (coder->param->status == BURNOUT)
		print_logs("has burnout", coder);
	pthread_mutex_unlock(&coder->param->status_lock);
	print_logs("is refactoring", coder);
	if (ft_usleep(coder->param->time_to_refactor, coder) != 0)
		return(1);
	return (0);
}
int check_dongle_status(t_dongle *dongle, int dongle_cooldown, t_coder *coder)
{
	int time_elapsed;
	if (dongle->init++ == 0)
		return(0);
	time_elapsed = current_time() - dongle->released_time;
	if(time_elapsed < dongle_cooldown)
		if(ft_usleep(dongle_cooldown - time_elapsed, coder) != 0)
			return(1);
	return(0);
}
int	get_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		check_dongle_status(coder->left_dongle, coder->param->dongle_cooldown, coder);
		print_logs("has taken a dongle", coder);
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		check_dongle_status(coder->right_dongle, coder->param->dongle_cooldown, coder);
		print_logs("has taken a dongle", coder);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		check_dongle_status(coder->right_dongle, coder->param->dongle_cooldown, coder);
		print_logs("has taken a dongle", coder);
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		check_dongle_status(coder->left_dongle, coder->param->dongle_cooldown, coder);
		print_logs("has taken a dongle", coder);
	}
	return (0);
}

int	drop_dongles(t_coder *coder)
{
	coder->left_dongle->released_time = current_time();
	coder->right_dongle->released_time = current_time();
	pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
	pthread_mutex_unlock(&coder->right_dongle->dongle_lock);
	return (0);
}

int	compilation(t_coder *coder)
{
	//mutex_lock to protect the data
	//mutex_unlock when data is finally modified
	get_dongles(coder);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->times_compiled += 1;
	coder->last_compiled = current_time();
	pthread_mutex_unlock(&coder->coder_mutex);
	print_logs("is compiling", coder);
	ft_usleep(coder->param->time_to_compile, coder);
	drop_dongles(coder);
	return (0);
}
