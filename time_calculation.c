/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_calculation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:47 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/15 22:39:52 by niverdie         ###   ########.fr       */
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
