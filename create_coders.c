/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_coders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:57:16 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/27 19:31:04 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int print_logs(char *action, t_coder *coder)
{
	pthread_mutex_lock(&coder->param->print_lock);
	printf("%ld %d %s\n", current_time(), coder->id, action);
	pthread_mutex_unlock(&coder->param->print_lock);
	return (0);
}

void *routine(void *arguments)
{
	t_coder *coder;
	coder = (t_coder *) arguments;
	t_param *param;
	param = coder->param;
	print_logs("is eating chicken", coder);
	return NULL;
}


int	initialization(t_param *param)
{
	int index;
	t_coder *coder;

	index = 0;
	coder = malloc(sizeof(t_coder)*(param->number_of_coders));
	if (pthread_mutex_init(&param->print_lock, NULL) != 0)
		clean_exit();
	while(index < (param->number_of_coders))
	{
		coder[index].param = param;
		coder[index].id = index + 1;
		if (pthread_create(&coder->coder, NULL, &routine, &coder[index]) != 0)
			clean_exit();
		index++;
	}
	index = 0;
	while(index < (param->number_of_coders))
	{
		pthread_join(coder->coder, NULL);
		index++;
	}
	return(0);
}
