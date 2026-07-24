/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:47:31 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/24 12:47:33 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	match_word(char *word1, char *word2)
{
	int	i;

	i = 0;
	while (word1[i] != 0 || word2[i] != 0)
	{
		if (word1[i] != word2[i])
			return (1);
		i++;
	}
	return (0);
}

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
