/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niverdie <niverdie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 12:44:33 by niverdie          #+#    #+#             */
/*   Updated: 2026/09/03 16:01:57 by niverdie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
// # include <stdlib.h>

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
	unsigned long		dongle_cooldown;
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
	int					has_compiled;
	pthread_t			coder;
	pthread_mutex_t		coder_mutex;
	t_param				*param;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
}						t_coder;

typedef struct s_slot
{
	int					coder_id;
	unsigned long		deadline;
	int					is_compiling;
}						t_slot;

// dongles
typedef struct s_dongle
{
	pthread_mutex_t		dongle_lock;
	int					released_time;
	int					init;
	t_slot				dongle_queue[2];
}						t_dongle;

int						parsing(int ac, char **av, t_param *param);
unsigned long			current_time(t_param *param);
int						initialization(t_param *param);

int						clean_exit(t_param *param, int step, int failed_idx);
int						print_logs(char *action, t_coder *coder);
int						debug(t_coder *coder);
int						refactor(t_coder *coder);
int						compilation(t_coder *coder);
void					join_threads(t_coder *coder_array, int index);
void					*monitor(void *arguments);
int						ft_usleep(unsigned long time_to_sleep, t_coder *coder);
int						is_burnout(t_coder *coder);
int						check_cooldown(t_coder *coder, t_dongle *dongle);
void					destroy_mutex_spe(t_param *param, int count);
void					destroy_dongles_mutexes(t_dongle *dongle_array,
							int count);
void					destroy_coder_mutexes(t_coder *coder_array, int count);
void					free_allocation(t_coder *coder_array,
							t_dongle *dongle_array);
int						mutex_init(t_param *param);
int						init_dongles(t_param *param, t_dongle *dongle);
int						alloc_memory(t_param *param);
int						init_coders_mutex(t_param *param, t_coder *coder);
int						get_dongles(t_coder *coder);
int						release_dongles(t_coder *coder);
void					update_dongle_queue(t_dongle *dongle, t_coder *coder);
#endif