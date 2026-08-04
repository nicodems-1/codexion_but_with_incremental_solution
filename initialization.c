/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:57:16 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/04 19:21:21 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	print_logs(char *action, t_coder *coder)
{
	pthread_mutex_lock(&coder->param->print_lock);
	printf("%ld %d %s\n", current_time(), coder->id, action);
	pthread_mutex_unlock(&coder->param->print_lock);
	return (0);
}

void	*routine(void *arguments)
{
	t_coder	*coder;

	coder = (t_coder *)arguments;
	compilation(coder);
	debug(coder);
	refactor(coder);
	return (NULL);
}
int	mutex_init(t_param *param)
{
	if (pthread_mutex_init(&param->print_lock, NULL) != 0)
		clean_exit();
	if (pthread_mutex_init(&param->status_lock, NULL) != 0)
		clean_exit();
	param->status = RUNNING;
	return(0);
}


int init_dongles(t_param *param, t_dongle *dongle)
{
	int			index;
	
	index = 0;
	while (index < (param->number_of_coders))
	{
		if (pthread_mutex_init(&dongle[index].dongle_lock, NULL) != 0)
		clean_exit();
		dongle[index].released_time = 0;
		dongle[index].init = 0;
		index++;
	}
	return(0);
}
int init_coders(t_param *param, t_coder *coder, t_dongle *dongle)
{
	int index;
	
	index = 0;
	while (index < (param->number_of_coders))
	{
		coder[index].left_dongle = &dongle[index];
		coder[index].right_dongle = &dongle[(index + 1)% param->number_of_coders];
		coder[index].param = param;
		coder[index].id = index + 1;
		if (pthread_mutex_init(&coder[index].coder_mutex, NULL) != 0)
			clean_exit();
		if (pthread_create(&coder[index].coder, NULL, &routine, &coder[index]) != 0)
			clean_exit();
		index++;
	}
	return(0);
}

int	initialization(t_param *param)
{
	int			index;
	t_coder		*coder;
	t_dongle	*dongle;
	// pthread_t monitor_thread;

	dongle = malloc(sizeof(t_dongle) * (param->number_of_coders));
	coder = malloc(sizeof(t_coder) * (param->number_of_coders));

	init_dongles(param, dongle);
	init_coders(param, coder, dongle);
	// pthread_create(&monitor_thread, NULL, &monitor, coder);
	index = 0;
	// pthread_join(monitor_thread, NULL);
	while (index < (param->number_of_coders))
	{
		pthread_join(coder[index].coder, NULL);
		index++;
	}
	return (0);
}
