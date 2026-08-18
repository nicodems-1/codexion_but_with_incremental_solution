/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:44:33 by niverdie          #+#    #+#             */
/*   Updated: 2026/08/18 17:05:02 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;

typedef enum e_status
{
	RUNNING,
	FINISHED,
	BURNOUT,
	ERROR
}						t_status;

// parameter + timing
typedef struct s_param
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					nb_compiles;
	int					dongle_cooldown;
	char				*scheduler;
	unsigned long		time_initialized;
	t_dongle			*dongles;
	t_coder				*coders;
	t_status			status;
	pthread_t			monitor_thread;
	int					unlock_race;
	pthread_cond_t		starting_race;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		lock_race;
	pthread_mutex_t		update_status;
	pthread_mutex_t		time_mutex;
}						t_param;

// coders with mutex
typedef struct s_coder
{
	int					id;
	int					last_compiled;
	int					times_compiled;
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
	int					released_time;
	int					init;
	int					priority_dongle_id;
	int					id_holding_dongle;
}						t_dongle;

int						match_word(char *word1, char *word2);
int						ft_is_number(char *number);
int						parsing(int ac, char **av, t_param *param);
int						initialization(t_param *param);
unsigned long			current_time(t_param *param);
void					clean_exit(t_coder *coder_array,
							t_dongle *dongle_array);
int						print_logs(char *action, t_coder *coder);
int						debug(t_coder *coder);
int						refactor(t_coder *coder);
int						compilation(t_coder *coder);
void					join_threads(t_coder *coder_array);
void					*monitor(void *arguments);
void					*ft_calloc(size_t nmemb, size_t size);
#endif