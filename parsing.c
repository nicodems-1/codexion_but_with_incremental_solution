/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:31:28 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/14 14:25:41 by niverdie         ###   ########.fr       */
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

	i = 1;
	while (i < 9)
	{
		converted[i - 1] = atoi(parsed[i]);
		if (converted[i - 1] < 0)
			return (i);
		i++;
	}
	param->number_of_coders = converted[0];
	printf("nb of coders == %d\n", param->number_of_coders);
	param->time_to_burnout = converted[1];
	printf("time to burnout == %d\n", param->time_to_burnout);
	param->time_to_compile = converted[2];
	printf("time to compile == %d\n", param->time_to_compile);
	param->time_to_debug = converted[3];
	printf("time to debug == %d\n", param->time_to_debug);
	param->time_to_refactor = converted[4];
	printf("time to refactor == %d\n", param->time_to_refactor);
	param->number_of_compiles_required = converted[5];
	printf("number of compiles == %d\n", param->number_of_compiles_required);
	param->dongle_cooldown = converted[6];
	printf("dongle cooldown == %d\n", param->dongle_cooldown);
	return (0);
}

int	basic_parsing(char **av)
{
	int	i;

	i = 1;
	while (i < 8)
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

int	parsing(int ac, char **av, t_param *param)
{
	int		index;

	if (ac != 9)
	{
		printf("Args count must be 8, current args count = %d\n", ac - 1);
		return (1);
	}
	if (basic_parsing(av) == 1)
		return (1);
	else if ((update_struct(av, param)) != 0)
	{
		index = update_struct(av, param);
		printf("Error, arg[%d]: \"%s\" is bigger than int max", index,
			av[index]);
		return (1);
	}
	else if (parse_flag(av[8], param) == 1)
	{
		printf("Arg[8]: \"%s\" not accepted, instead try: \"edf\" or \"fifo\"",
			av[8]);
		return (1);
	}
	printf("flag == %s\n", param->scheduler);
	return (0);
}
