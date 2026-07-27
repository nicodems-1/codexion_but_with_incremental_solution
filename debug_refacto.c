/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_refacto.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:24 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/28 01:50:10 by niverdie         ###   ########.fr       */
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
