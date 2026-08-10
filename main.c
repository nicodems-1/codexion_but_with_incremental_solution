/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 13:03:36 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/10 17:02:04 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	main(int ac, char **av)
{
	t_param	*param;

	param = malloc(sizeof(t_param));
	if(!param)
	{
		free(param);
		exit(1);
	}
	param = parsing(ac, av, param);
	if(&parsing == NULL)
		return(1);
	initialization(param);
	free(param);
}
