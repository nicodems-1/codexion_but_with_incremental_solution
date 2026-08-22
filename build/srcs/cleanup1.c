/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:29 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/21 22:39:13 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

static int	get_count(int current_step, int failed_step, int failed_idx,
		int total)
{
	if (current_step == failed_step)
		return (failed_idx);
	return (total);
}

int	clean_exit(t_param *param, int step, int failed_idx)
{
	t_coder		*coder;
	t_dongle	*dongle;
	int			total;

	coder = param->coders;
	dongle = param->dongles;
	total = param->number_of_coders;
	if (step >= 6)
		pthread_join(param->monitor_thread, NULL);
	if (step >= 5)
		join_threads(coder, get_count(5, step, failed_idx, total));
	if (step >= 4)
		destroy_mutex_spe(param, get_count(5, step, failed_idx, total));
	if (step >= 3)
		destroy_dongles_mutexes(dongle, get_count(3, step, failed_idx, total));
	if (step >= 2)
		destroy_coder_mutexes(coder, get_count(2, step, failed_idx, total));
	if (step >= 1)
		free_allocation(coder, dongle);
	if (step <= 6)
		return (1);
	else
		return (0);
}
