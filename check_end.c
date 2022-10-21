/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_end.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 14:55:34 by pfrances          #+#    #+#             */
/*   Updated: 2022/10/22 00:15:03 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_memory(t_config *config)
{
	free(config->philos);
	free(config->forks);
}

size_t	get_time(struct timeval start)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((time.tv_sec * 1000000 + time.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec)));
}

void	stop_game(t_config *config)
{
	size_t	i;

	config->can_continue = false;
	i = 0;
	while (i < config->rules.nb_philo)
	{
		pthread_mutex_destroy(&config->forks[i]);
		pthread_detach(config->philos[i].thread_id);
		i++;
	}
	pthread_mutex_destroy(&config->check_forks);
	free_memory(config);
}

bool	all_meals_eaten(t_config *config)
{
	size_t	i;

	i = 0;
	while (i < config->rules.nb_philo)
	{
		if (config->philos[i].nbr_ate_meals < config->rules.nbr_of_meals)
			return (false);
		i++;
	}
	return (true);
}

void	check_end(t_config *config)
{
	size_t	i;

	while (1)
	{
		i = 0;
		config->time_us = get_time(config->start_time);
		config->time_ms = config->time_us / 1000;
		while (i < config->rules.nb_philo)
		{
			if (config->time_us - config->philos[i].last_meal
				> config->rules.time_to_die)
			{
				printf("%ld %ld died\n", config->time_ms, i + 1);
				stop_game(config);
				return ;
			}
			if (all_meals_eaten(config) == true)
			{
				stop_game(config);
				return ;
			}
			i++;
		}
	}
}
