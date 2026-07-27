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

void *routine()
{
	printf("Test from coders\n");
	return NULL;
}

int	initialization(t_param *param)
{
	int index;
	t_coder *coders;

	index = 0;
	coders = malloc(sizeof(t_coder)*(param->number_of_coders));
	while(index < (param->number_of_coders - 1))
	{
		pthread_create(&coders->coder, NULL, &routine, NULL);
		pthread_mutex_init(&coders->coder_lock, NULL);
		index++;
	}
	index = 0;
	while(index < (param->number_of_coders - 1))
	{
		pthread_join(coders->coder, NULL);
		index++;
	}
	printf("%d\n", index);
	return(0);
}
