/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:31:28 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/24 13:06:58 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	valid_length(char *number)
{
	int	i;

	i = 0;
	while (number[i])
		i++;
	if (i > 10)
		return (1);
	return (0);
}

int	update_struct(char **parsed, t_param *param)
{
	int	i;
	int	converted[8];

	i = 0;
	while (i < 9)
	{
		converted[i - 1] = atoi(parsed[i]);
		if (converted[i - 1] < 0)
			return (i);
		i++;
	}
	param->number_of_coders = converted[0];
	param->time_to_burnout = converted[1];
	param->time_to_compile = converted[2];
	param->time_to_debug = converted[3];
	param->time_to_refactor = converted[4];
	param->number_of_compiles_required = converted[5];
	param->dongle_cooldown = converted[6];
	param->dongle_cooldown = converted[7];
	return (0);
}

int	basic_parsing(char **av)
{
	int	i;

	i = 1;
	while (i != 9)
	{
		if (valid_length(av[i]) == 1)
		{
			printf("Arg[%d]: \"%s\" length is too long", i, av[i]);
			return (1);
		}
		if (ft_is_number(av[i]) == 1)
		{
			printf("Arg[%d]: \"%s\" is not a number", i, av[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	parse_flag(char *flag, t_param *param)
{
	if ((match_word("fifo", flag) == 0) || match_word("edf", flag) == 0)
	{
		param->scheduler = flag;
		return (0);
	}
	return (1);
}

t_param	*parsing(int ac, char **av, t_param *param)
{
	int		index;

	if (ac != 10)
	{
		printf("Args count must be 9, current args count = %d\n", ac - 1);
		exit(1);
	}
	if (basic_parsing(av) == 1)
		exit(1);
	else if ((update_struct(av, param)) != 0)
	{
		index = update_struct(av, param);
		printf("Error, arg[%d]: \"%s\" is bigger than int max", index,
			av[index]);
		exit(1);
	}
	else if (parse_flag(av[9], param) == 1)
	{
		printf("Arg[9]: \"%s\" not accepted, instead try: \"edf\" or \"fifo\"",
			av[9]);
		exit(1);
	}
	return (param);
}
