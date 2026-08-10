/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:29 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/10 14:03:07 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

void	clean_exit(t_coder *coder_array, t_dongle *dongle_array)
{
	t_param *param;
	int	i;

	i = 0;
	param = coder_array[0].param;
	printf("cleaning and exiting\n");
	//join all the threads ?? 
	//destroy all the mutex
	pthread_mutex_destroy(&param->update_status);
	pthread_mutex_destroy(&param->status_lock);
	pthread_mutex_destroy(&param->lock_race);
	pthread_mutex_destroy(&param->print_lock);
	while(i++ != param->number_of_coders)
	{
		pthread_mutex_destroy(&coder_array[i].coder_mutex);
		pthread_mutex_destroy(&dongle_array[i].dongle_lock);
		free(&dongle_array[i]);
		free(&coder_array[i]);
	}
	//free all the malloc
	free(param);
	free(coder_array);
	free(dongle_array);
	exit(1);
}
