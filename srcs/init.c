/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 17:29:48 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/21 14:31:45 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	set_philos_info(t_config *config)
{
	size_t	i;
	size_t	nbr_philo;
	t_philo	*philo;

	i = 0;
	nbr_philo = config->info.nb_philo;
	while (i < nbr_philo)
	{
		philo = &config->philos[i];
		philo->philo_id = i + 1;
		philo->left_philo = &config->philos[(i + 1) % nbr_philo];
		philo->left_forks = &config->forks[i];
		philo->right_forks = &config->forks[(i + 1) % nbr_philo];
		philo->right_philo = &config->philos[(i + nbr_philo - 1) % nbr_philo];
		philo->check_forks = &config->info.check_forks;
		philo->info = &config->info;
		philo->state = IS_THINKING;
		philo->timmings.last_meal_timming = 0;
		philo->nbr_ate_meals = 0;
		philo->meals_to_eat = config->info.total_meals / nbr_philo;
		set_philos_timming(philo, &config->info.default_timmings);
		i++;
	}
}

bool	set_philo_and_forks(t_config *config)
{
	size_t	i;

	config->philos = malloc(sizeof(t_philo) * config->info.nb_philo);
	if (config->philos == NULL)
		return (false);
	config->forks = malloc(sizeof(pthread_mutex_t) * config->info.nb_philo);
	if (config->forks == NULL)
	{
		free(config->philos);
		return (false);
	}
	set_philos_info(config);
	pthread_mutex_init(&config->info.check_forks, NULL);
	pthread_mutex_init(&config->info.writing_logs_access, NULL);
	i = 0;
	while (i < config->info.nb_philo)
	{
		pthread_mutex_init(&config->forks[i], NULL);
		i++;
	}
	return (true);
}

bool	set_global_infos(t_global_info *info, char **argv)
{
	if (set_nbr(&info->nb_philo, argv[1]) == false)
		return (false);
	if (set_default_timming(&info->default_timmings, argv) == false)
		return (false);
	if (argv[5] != NULL)
	{
		if (set_nbr(&info->total_meals, argv[5]) == false)
			return (false);
		info->total_meals *= info->nb_philo;
	}
	else
		info->total_meals = SIZE_MAX;
	info->can_continue = true;
	info->ready_to_start = false;
	return (true);
}

bool	init(t_config *config, char **argv)
{
	if (set_global_infos(&config->info, argv) == false)
		return (false);
	if (set_philo_and_forks(config) == false)
		return (false);
	return (true);
}
