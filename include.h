/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:44:33 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/02 13:43:01 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle	t_dongle;

typedef enum e_status
{
	RUNNING,
	FINISHED,
	BURNOUT
} status;

// parameter + timing
typedef struct s_param
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	char				*scheduler;
	pthread_mutex_t		print_lock;
}						t_param;

// coders with mutex
typedef struct s_coder
{
	int					id;
	int	last_compiled;
	int times_compiled;
	pthread_t			coder;
	pthread_mutex_t		coder_mutex;
	t_param				*param;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
}						t_coder;

// dongles
typedef struct s_dongle
{
	pthread_mutex_t		dongle_lock;
	int	released_time;
}						t_dongle;


int						match_word(char *word1, char *word2);
int						ft_is_number(char *number);
t_param					*parsing(int ac, char **av, t_param *param);
int						initialization(t_param *param);
unsigned long			current_time(void);
int						clean_exit(void);
int						print_logs(char *action, t_coder *coder);
int						debug(t_coder *coder);
int						refactor(t_coder *coder);
int						compilation(t_coder *coder);
#endif