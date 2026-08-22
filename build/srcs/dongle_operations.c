/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 03:20:09 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 03:34:04 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

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

int	take_one_dongle(t_dongle *dongle, t_coder *coder)
{
	update_dongle_queue(dongle, coder);
	if (wait_for_dongle(dongle, coder) == 1)
		return (1);
	pthread_mutex_lock(&dongle->dongle_lock);
	dongle->dongle_queue[0].is_compiling = 1;
	pthread_mutex_unlock(&dongle->dongle_lock);
	print_logs("has taken a dongle", coder);
	return (0);
}

int	get_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		if (take_one_dongle(coder->left_dongle, coder) == 1)
			return (1);
		if (take_one_dongle(coder->right_dongle, coder) == 1)
			return (1);
	}
	else
	{
		if (take_one_dongle(coder->right_dongle, coder) == 1)
			return (1);
		if (take_one_dongle(coder->left_dongle, coder) == 1)
			return (1);
	}
	return (0);
}

int	release_dongles(t_coder *coder)
{
	t_slot	*queue;

	pthread_mutex_lock(&coder->left_dongle->dongle_lock);
	queue = coder->left_dongle->dongle_queue;
	queue[0].coder_id = queue[1].coder_id;
	queue[1].coder_id = -1;
	coder->left_dongle->released_time = current_time(coder->param);
	queue[0].is_compiling = 0;
	queue[0].deadline = queue[1].deadline;
	pthread_mutex_unlock(&coder->left_dongle->dongle_lock);
	pthread_mutex_lock(&coder->right_dongle->dongle_lock);
	queue = coder->right_dongle->dongle_queue;
	coder->right_dongle = coder->right_dongle;
	queue[0].coder_id = queue[1].coder_id;
	queue[1].coder_id = -1;
	coder->right_dongle->released_time = current_time(coder->param);
	queue[0].is_compiling = 0;
	queue[0].deadline = queue[1].deadline;
	pthread_mutex_unlock(&coder->right_dongle->dongle_lock);
	return (0);
}
