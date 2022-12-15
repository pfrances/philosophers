/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:52:44 by pfrances          #+#    #+#             */
/*   Updated: 2022/12/15 12:00:09 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	do_sleep(t_philo *philo)
{
	if (philo->info->can_continue == false)
		return (false);
	philo->state = IS_SLEEPING;
	if (writing_logs(philo, SLEEP_MSG) == false)
		return (false);
	if (ZURUI)
		sleep_until(philo, philo->timmings.start_delay \
			+ (philo->nbr_ate_meals * philo->timmings.time_to_one_circuit) \
			- philo->timmings.time_to_think);
	else
		sleep_until(philo, philo->timmings.last_meal_timming \
			+ philo->timmings.time_to_eat + philo->timmings.time_to_sleep);
	return (true);
}

bool	do_think(t_philo *philo)
{
	if (philo->info->can_continue == false)
		return (false);
	philo->state = IS_THINKING;
	if (writing_logs(philo, THINK_MSG) == false)
		return (false);
	sleep_until(philo, philo->timmings.start_delay \
		+ philo->nbr_ate_meals * philo->timmings.time_to_one_circuit);
	return (true);
}

bool	wait_before_starting(t_philo *philo)
{
	while (philo->info->ready_to_start == false)
		usleep(1);
	if (philo->timmings.start_delay == 0)
		return (true);
	philo->state = IS_THINKING;
	if (writing_logs(philo, THINK_MSG) == false)
		return (false);
	sleep_until(philo, philo->timmings.start_delay);
	return (true);
}

void	*philo_daily_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (wait_before_starting(philo) == false)
		return (NULL);
	while (philo->info->total_meals > 0)
	{
		if (eat(philo) == false)
			return (NULL);
		if (do_sleep(philo) == false)
			return (NULL);
		if (do_think(philo) == false)
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_config	config;
	size_t		i;

	if (argc >= 4 && 5 <= argc && init(&config, argv))
	{
		i = 0;
		while (i < config.info.nb_philo)
		{
			if (pthread_create(&config.philos[i].thread_id, NULL,
					&philo_daily_life, &config.philos[i]) != 0)
				return (0);
			i++;
		}
		i = 0;
		philos_monitoring(&config);
	}
	return (0);
}
