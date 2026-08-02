/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:24 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/02 13:30:30 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	ft_usleep(unsigned long time_to_sleep)
{
	unsigned long	begin_time;
	unsigned long	end_time;

	begin_time = current_time();
	end_time = begin_time + time_to_sleep;
	while (current_time() < end_time)
	{
		usleep(10);
	}
	return (0);
}

int	debug(t_coder *coder)
{
	print_logs("is debugging", coder);
	ft_usleep(coder->param->time_to_debug);
	return (0);
}

int	refactor(t_coder *coder)
{
	print_logs("is refactoring", coder);
	ft_usleep(coder->param->time_to_refactor);
	return (0);
}

int check_dongle_status(t_dongle *dongle)
{
	printf("check dongle status: %d\n", dongle->taken);
	if(dongle->taken == 0)
	{
		dongle->taken = 1;
		return (0);
	}
	else
		return(1);
}

int	get_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		if (check_dongle_status(coder->left_dongle) == 0)
			print_logs("has taken a dongle", coder);
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		if (check_dongle_status(coder->right_dongle) == 0)
			print_logs("has taken a dongle", coder);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		if (check_dongle_status(coder->right_dongle) == 0)
		print_logs("has taken a dongle", coder);
		coder->right_dongle->taken = 0;
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		if (check_dongle_status(coder->right_dongle) == 0)
			print_logs("has taken a dongle", coder);
	}
	return (0);
}

int	drop_dongles(t_coder *coder)
{
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
	ft_usleep(coder->param->time_to_compile);
	print_logs("is compiling", coder);
	drop_dongles(coder);
	return (0);
}
