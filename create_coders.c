/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_coders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:57:16 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/24 13:07:19 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int print_logs(char *action, t_coder *coder)
{
	int total_time = 400;
	printf("%d  %d  %s\n", total_time, coder->id, action);
	return (0);
}

void *routine(void *arguments)
{
	print_logs("is testing", arguments);
	return NULL;
}

int	initialization(t_param *param)
{
	int index;
	t_coder *coder;

	index = 0;
	coder = malloc(sizeof(t_coder)*(param->number_of_coders));
	while(index < (param->number_of_coders - 1))
	{
		pthread_create(&coder->coder, NULL, &routine, coder);
		pthread_mutex_init(&coder->coder_lock, NULL);
		coder->id = index;
		index++;
	}
	index = 0;
	while(index < (param->number_of_coders - 1))
	{
		pthread_join(coder->coder, NULL);
		index++;
	}
	printf("%d\n", index);
	return(0);
}
