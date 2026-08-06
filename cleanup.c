/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 01:44:29 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/06 16:42:06 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

void	clean_exit(void)
{
	printf("cleaning and exiting\n");
	//join all the threads ?? 
	//destroy all the mutex
	//free all the malloc
	exit(1);
}
