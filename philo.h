/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:52:55 by pfrances          #+#    #+#             */
/*   Updated: 2022/10/21 23:20:17 by pfrances         ###   ########.fr       */
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

typedef enum e_philo_state
{
	IS_EATING,
	IS_SLEEPING,
	IS_THINKING
}		t_philo_state;

typedef struct s_rules
{
	size_t	nb_philo;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	nbr_of_meals;
}		t_rules;

typedef struct s_philo
{
	struct s_philo	*left_philo;
	struct s_philo	*right_philo;
	pthread_mutex_t	*left_forks;
	pthread_mutex_t	*right_forks;
	pthread_mutex_t	*check_forks;
	size_t			id;
	size_t			last_meal;
	size_t			nbr_ate_meals;
	t_philo_state	state;
	pthread_t		thread_id;
	size_t			*time_us;
	size_t			*time_ms;
	t_rules			*rules;
	bool			*can_continue;
	struct s_config	*game;
}		t_philo;

typedef struct s_config
{
	t_rules			rules;
	pthread_mutex_t	*forks;
	pthread_mutex_t	check_forks;
	t_philo			*philos;
	struct timeval	start_time;
	size_t			time_us;
	size_t			time_ms;
	bool			can_continue;
}		t_config;

/*		init.c					*/
bool	init(t_config *game, char **argv);

/*		check_death.c			*/
void	check_end(t_config *game);

/*		meals.c					*/
void	eat(t_philo *philo);

/*		atoi_error_check.c		*/
int		ft_atoi_with_error_check(const char *nptr, bool *error_flag);

#endif