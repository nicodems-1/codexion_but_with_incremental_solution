/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:29 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/31 14:04:19 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	clean_exit(void)
{
	printf("cleaning and exiting");
	//join all the threads ?? 
	//destroy all the mutex
	//free all the malloc
	return (1);
}
