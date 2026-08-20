/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 13:03:36 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/19 20:53:35 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	main(int ac, char **av)
{
	t_param	*param;

	param = malloc(sizeof(t_param));
	if (!param)
	{
		free(param);
		return (1);
	}
	memset(param, 0, sizeof(t_param));
	if (parsing(ac, av, param) == 1)
	{
		free(param);
		return (1);
	}
	initialization(param);
	free(param);
}
