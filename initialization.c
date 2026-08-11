/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:57:16 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/11 15:27:50 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	print_logs(char *action, t_coder *coder)
{
	pthread_mutex_lock(&coder->param->print_lock);
	if (coder->param->status != BURNOUT)
		printf("%ld %d %s\n", current_time(), coder->id, action);
	pthread_mutex_unlock(&coder->param->print_lock);
	return (0);
}
void	*routine(void *arguments)
{
	t_coder	*coder;

	coder = (t_coder *)arguments;
	pthread_mutex_lock(&coder->param->lock_race);
	while (coder->param->unlock_race != 1)
		pthread_cond_wait(&coder->param->starting_race,
			&coder->param->lock_race);
	pthread_mutex_unlock(&coder->param->lock_race);
	while (1)
	{
		if (coder->param->status != RUNNING)
			return (NULL);
		compilation(coder);
		if (coder->param->status != RUNNING)
			return (NULL);
		debug(coder);
		if (coder->param->status != RUNNING)
			return (NULL);
		refactor(coder);
	}
	return (NULL);
}
int	mutex_init(t_param *param)
{
	int error;
	error = 0;
	if (pthread_mutex_init(&param->print_lock, NULL) != 0)
		error = 1;
	if (pthread_mutex_init(&param->status_lock, NULL) != 0)
		error = 1;
	return (error);
}

int	init_dongles(t_param *param, t_dongle *dongle)
{
	int	index;

	index = 0;
	while (index < (param->number_of_coders))
	{
		if (pthread_mutex_init(&dongle[index].dongle_lock, NULL) != 0)
			return(1);
		dongle[index].released_time = 0;
		dongle[index].init = 0;
		index++;
	}
	return (0);
}
int	init_coders(t_param *param, t_coder *coder, t_dongle *dongle)
{
	int	index;

	index = 0;
	if (pthread_mutex_init(&param->status_lock, NULL) != 0)
			return(1);
	param->status = RUNNING;
	while (index < (param->number_of_coders))
	{
		coder[index].left_dongle = &dongle[index];
		coder[index].right_dongle = &dongle[(index + 1)
			% param->number_of_coders];
		coder[index].param = param;
		coder[index].id = index + 1;
		if (pthread_mutex_init(&coder[index].coder_mutex, NULL) != 0)
			return(1);
		if (pthread_create(&coder[index].coder, NULL, &routine,
				&coder[index]) != 0)
			return(1);
		index++;
	}
	return (0);
}

int	initialization(t_param *param)
{
	t_coder		*coder;
	t_dongle	*dongle;
	pthread_t	monitor_thread;

	param->unlock_race = 0;
	pthread_cond_init(&param->starting_race, NULL);
	dongle = ft_calloc( (param->number_of_coders), sizeof(t_dongle));
	coder = ft_calloc((param->number_of_coders), sizeof(t_coder));
	init_dongles(param, dongle);
	init_coders(param, coder, dongle);
	param->dongles = dongle;
	param->coders  = coder;
	pthread_create(&monitor_thread, NULL, &monitor, coder);
	param->monitor_thread = monitor_thread;
	param->unlock_race = 1;
	pthread_cond_broadcast(&param->starting_race);
	clean_exit(coder, dongle);
	exit(0);
	return (0);
}
