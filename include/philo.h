/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:52:55 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/21 14:32:54 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <limits.h>
# include <sys/time.h>
# define TAKE_FORK_MSG "has taken a fork\n"
# define EAT_MSG "is eating\n"
# define SLEEP_MSG "is sleeping\n"
# define THINK_MSG "is thinking\n"
# define DIED "died\n"

typedef enum e_wich_forks
{
	NONE,
	LEFT,
	RIGHT,
	BOTH
}		t_wich_forks;

typedef enum e_philo_state
{
	IS_EATING,
	IS_SLEEPING,
	IS_THINKING
}		t_philo_state;

typedef struct s_timmings
{
	size_t			time_us;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			last_meal_timming;
	size_t			time_to_sleep;
	size_t			time_to_think;
	size_t			time_to_one_circuit;
	size_t			start_delay;
}		t_timmings;

typedef struct s_global_info
{
	size_t			nb_philo;
	t_timmings		default_timmings;
	struct timeval	start_time;
	bool			ready_to_start;
	size_t			total_meals;
	pthread_mutex_t	check_forks;
	pthread_mutex_t	writing_logs_access;
	bool			can_continue;
}		t_global_info;

typedef struct s_philo
{
	size_t			philo_id;
	pthread_t		thread_id;
	t_philo_state	state;
	struct s_philo	*left_philo;
	struct s_philo	*right_philo;
	pthread_mutex_t	*check_forks;
	pthread_mutex_t	*left_forks;
	pthread_mutex_t	*right_forks;
	size_t			meals_to_eat;
	size_t			nbr_ate_meals;
	t_timmings		timmings;
	t_global_info	*info;
}		t_philo;

typedef struct s_config
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	t_global_info	info;
}		t_config;

/*		init.c					*/
bool	init(t_config *game, char **argv);

/*		init_philo.c			*/
void	set_philos_info(t_config *config);
bool	set_philo_and_forks(t_config *config);

/*		check_death.c			*/
void	philos_monitoring(t_config *game);

/*		meals.c					*/
bool	eat(t_philo *philo);

/*		timing.c				*/
bool	set_default_timming(t_timmings *default_timmings, char **argv);
void	set_philos_timming(t_philo *philo, t_timmings *default_timmings);
size_t	get_time(struct timeval start);
void	update_time(t_philo *philo);
void	sleep_until(t_philo *philo, size_t time_to_awake);

/*		set_nbr.c				*/
bool	set_nbr(size_t	*nbr_dst, char *nbr);

/*		writing_logs.c			*/
bool	writing_logs(t_philo *philo, char *msg);

#endif