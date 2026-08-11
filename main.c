/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 13:03:36 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/11 12:16:52 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	main(int ac, char **av)
{
	t_param	*param;

	param = malloc(sizeof(t_param));
	param = parsing(ac, av, param);
	initialization(param);
	free(param);
}
