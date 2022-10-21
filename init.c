/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 20:23:57 by pfrances          #+#    #+#             */
/*   Updated: 2022/10/22 00:14:05 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	set_nbr(size_t	*nbr_dst, char *nbr)
{
	bool	error_flag;

	error_flag = true;
	*nbr_dst = (size_t)ft_atoi_with_error_check(nbr, &error_flag);
	if (error_flag == true)
		return (false);
	return (true);
}

bool	set_rules(t_rules *rules, char **argv)
{
	if (set_nbr(&rules->nb_philo, argv[1]) == false)
		return (false);
	if (set_nbr(&rules->time_to_die, argv[2]) == false)
		return (false);
	rules->time_to_die *= 1000;
	if (set_nbr(&rules->time_to_eat, argv[3]) == false)
		return (false);
	rules->time_to_eat *= 1000;
	if (set_nbr(&rules->time_to_sleep, argv[4]) == false)
		return (false);
	rules->time_to_sleep *= 1000;
	if (argv[5] != NULL)
	{
		if (set_nbr(&rules->nbr_of_meals, argv[5]) == false)
			return (false);
	}
	else
		rules->nbr_of_meals = SIZE_MAX;
	return (true);
}

bool	do_allocation(t_config *config)
{
	config->philos = malloc(sizeof(t_philo) * config->rules.nb_philo);
	if (config->philos == NULL)
		return (false);
	config->forks = malloc(sizeof(pthread_mutex_t) * config->rules.nb_philo);
	if (config->forks == NULL)
	{
		free(config->philos);
		return (false);
	}
	return (true);
}

void	set_philos_info(t_config *config)
{
	size_t	i;
	size_t	nbr_philo;
	t_philo	*philo;

	i = 0;
	nbr_philo = config->rules.nb_philo;
	while (i < nbr_philo)
	{
		philo = &config->philos[i];
		philo->id = i + 1;
		philo->left_philo = &config->philos[(i + 1) % nbr_philo];
		philo->left_forks = &config->forks[i];
		philo->right_forks = &config->forks[(i + 1) % nbr_philo];
		philo->right_philo = &config->philos[(i + nbr_philo - 1) % nbr_philo];
		philo->check_forks = &config->check_forks;
		philo->state = IS_THINKING;
		philo->rules = &config->rules;
		philo->can_continue = &config->can_continue;
		philo->last_meal = 0;
		philo->nbr_ate_meals = 0;
		philo->time_us = &config->time_us;
		philo->time_ms = &config->time_ms;
		i++;
	}
}

bool	init(t_config *config, char **argv)
{
	size_t	i;

	if (set_rules(&config->rules, argv) == false)
		return (false);
	gettimeofday(&config->start_time, NULL);
	config->time_us = 0;
	config->time_ms = 0;
	if (do_allocation(config) == false)
		return (false);
	set_philos_info(config);
	config->can_continue = true;
	pthread_mutex_init(&config->check_forks, NULL);
	i = 0;
	while (i < config->rules.nb_philo)
	{
		pthread_mutex_init(&config->forks[i], NULL);
		i++;
	}
	return (true);
}
