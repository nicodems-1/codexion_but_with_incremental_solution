/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:31:28 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/22 03:23:58 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	ft_is_number(char *number)
{
	int	i;

	i = 0;
	while (number[i] != 0)
	{
		if ((number[i] > '9' || number[i] < '0'))
			return (1);
		i++;
	}
	return (0);
}

int	update_struct(char **parsed, t_param *param)
{
	int	i;
	int	converted[8];

	i = 1;
	while (i < 9)
	{
		converted[i - 1] = atoi(parsed[i]);
		if (converted[i - 1] < 0 || ((strcmp(parsed[i], "2147483647") > 0)
				&& strlen(parsed[i]) == 10))
			return (i);
		i++;
	}
	param->number_of_coders = converted[0];
	param->time_to_burnout = converted[1];
	param->time_to_compile = converted[2];
	param->time_to_debug = converted[3];
	param->time_to_refactor = converted[4];
	param->nb_compiles = converted[5];
	param->dongle_cooldown = converted[6];
	return (0);
}

int	basic_parsing(char **av)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (strlen(av[i]) > 10)
		{
			printf("Arg[%d]: \"%s\" length is too long\n", i, av[i]);
			return (1);
		}
		if (ft_is_number(av[i]) == 1)
		{
			printf("Arg[%d]: \"%s\" is not a number\n", i, av[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	parse_flag(char *flag, t_param *param)
{
	if ((strcmp("fifo", flag) == 0) || strcmp("edf", flag) == 0)
	{
		param->scheduler = flag;
		return (0);
	}
	return (1);
}

int	parsing(int ac, char **av, t_param *param)
{
	int	index;

	if (ac != 9)
		return (printf("Args count must be 8, current args count = %d\n", ac
				- 1), 1);
	if (basic_parsing(av) == 1)
		return (1);
	else if ((update_struct(av, param)) != 0)
	{
		index = update_struct(av, param);
		printf("Error, arg[%d]: \"%s\" is bigger than int max\n", index,
			av[index]);
		return (1);
	}
	else if (param->number_of_coders <= 0 || param->number_of_coders > 500)
	{
		printf("You must give a nb of coder between 0 and 500, current == %d\n",
			param->number_of_coders);
		return (1);
	}
	else if (parse_flag(av[8], param) == 1)
		return (printf("Arg[8]: \"%s\" wrong: \"edf\" or \"fifo\"\n", av[8]),
			1);
	return (0);
}
