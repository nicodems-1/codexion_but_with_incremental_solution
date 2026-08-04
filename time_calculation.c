/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_calculation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:47 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/04 12:09:31 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

unsigned long	current_time(void)
{
	static unsigned long	starting_time = 0;
	unsigned long			current;
	struct timeval			tv;

	gettimeofday(&tv, NULL);
	if (starting_time == 0)
		starting_time = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000));
	current = ((tv.tv_usec / 1000) + (tv.tv_sec * 1000)) - starting_time;
	return (current);
}
