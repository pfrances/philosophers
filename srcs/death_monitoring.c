/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitoring.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 14:55:34 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/20 16:45:46 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	free_memory(t_config *config)
{
	free(config->philos);
	free(config->forks);
}

void	stop_game(t_config *config)
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
	free_memory(config);
}

void	check_end(t_config *config)
{
	size_t		i;
	t_timmings	time;

	while (config->info.total_meals > 0)
	{
		usleep(100);
		i = 0;
		while (i < config->info.nb_philo)
		{
			time = config->philos[i].timmings;
			update_time(&config->philos[i]);
			if (time.time_us - time.last_meal_timming > time.time_to_die + 1000)
			{
				pthread_mutex_lock(&config->info.writing_logs_access);
				config->info.can_continue = false;
				printf("%ld %ld %s", time.time_us / 1000, i + 1, DIED);
				pthread_mutex_unlock(&config->info.writing_logs_access);
				stop_game(config);
				return ;
			}
			i++;
		}
	}
	config->info.can_continue = false;
	stop_game(config);
}
