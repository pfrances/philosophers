/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitoring.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 14:55:34 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/21 15:37:46 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	free_memory(t_config *config)
{
	free(config->philos);
	free(config->forks);
}

void	stop_simulation(t_config *config)
{
	size_t	i;

	i = 0;
	while (i < config->info.nb_philo)
	{
		pthread_join(config->philos[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < config->info.nb_philo)
	{
		pthread_mutex_destroy(&config->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&config->info.check_forks);
	pthread_mutex_destroy(&config->info.writing_logs_access);
	pthread_mutex_destroy(&config->info.decrease_total_meals);
	free_memory(config);
}

bool	check_and_write_death(t_config *config)
{
	size_t		i;
	t_timmings	time;

	i = 0;
	while (i < config->info.nb_philo)
	{
		time = config->philos[i].timmings;
		update_time(&config->philos[i]);
		pthread_mutex_lock(&config->philos[i].info->writing_logs_access);
		if (time.time_us - time.last_meal_timming > time.time_to_die + 1000)
		{
			if (config->philos[i].info->total_meals > 0)
				printf("%ld %ld %s",
					config->philos[i].timmings.time_us / 1000,
					config->philos[i].philo_id, DIED);
			config->philos[i].info->can_continue = false;
			pthread_mutex_unlock(&config->philos[i].info->writing_logs_access);
			stop_simulation(config);
			return (true);
		}
		pthread_mutex_unlock(&config->philos[i].info->writing_logs_access);
		i++;
	}
	return (false);
}

void	philos_monitoring(t_config *config)
{
	gettimeofday(&config->info.start_time, NULL);
	config->info.ready_to_start = true;
	while (config->info.total_meals > 0)
	{
		if (check_and_write_death(config) == true)
			return ;
		usleep(100);
	}
	pthread_mutex_lock(&config->info.writing_logs_access);
	config->info.can_continue = false;
	pthread_mutex_unlock(&config->info.writing_logs_access);
	stop_simulation(config);
}
