/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:44:33 by niverdie          #+#    #+#             */
/*   Updated: 2026/07/27 19:39:56 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

//parameter + timing
typedef struct s_param
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
	pthread_mutex_t coder_lock;
	pthread_mutex_t print_lock;
}			t_param;


//coders with mutex
typedef struct s_coder
{	
	pthread_t coder;
	int id;
	t_param *param;
}	t_coder;

//dongles

int			match_word(char *word1, char *word2);
int			ft_is_number(char *number);
t_param		*parsing(int ac, char **av, t_param *param);
int			initialization(t_param *param);
unsigned long current_time();
int clean_exit();
#endif