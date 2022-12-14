/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timming.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 00:43:28 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/24 14:19:17 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	set_default_timming(t_timmings *default_timmings, char **argv)
{
	if (set_nbr(&default_timmings->time_to_die, argv[2]) == false)
		return (false);
	default_timmings->time_to_die *= 1000;
	if (set_nbr(&default_timmings->time_to_eat, argv[3]) == false)
		return (false);
	default_timmings->time_to_eat *= 1000;
	if (set_nbr(&default_timmings->time_to_sleep, argv[4]) == false)
		return (false);
	default_timmings->time_to_sleep *= 1000;
	default_timmings->time_to_think = 0;
	default_timmings->time_us = 0;
	default_timmings->start_delay = 0;
	default_timmings->last_meal_timming = 0;
	return (true);
}

void	set_philos_timming(t_philo *philo, t_timmings *default_timmings)
{
	t_timmings	*time;

	time = &philo->timmings;
	*time = *default_timmings;
	if (philo->philo_id % 2 == 0)
			time->start_delay = time->time_to_eat;
	else if (philo->philo_id == philo->info->nb_philo)
		time->start_delay = time->time_to_eat * 2;
	if (philo->info->nb_philo % 2 == 0)
	{
		if (time->time_to_eat > time->time_to_sleep)
			time->time_to_think = time->time_to_eat - time->time_to_sleep;
	}
	else
	{
		if (time->time_to_eat * 2 > time->time_to_sleep)
			time->time_to_think = time->time_to_eat * 2 - time->time_to_sleep;
	}
	time->time_to_one_circuit = time->time_to_eat \
		+ time->time_to_sleep + time->time_to_think;
	if (time->time_to_die > time->time_to_one_circuit)
		time->start_delay += (philo->philo_id * 100) \
			% ((time->time_to_die - time->time_to_one_circuit) / 2);
}

size_t	get_time(struct timeval start)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((time.tv_sec * 1000000 + time.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec)));
}

void	update_time(t_philo *philo)
{
	philo->timmings.time_us = get_time(philo->info->start_time);
}

void	sleep_until(t_philo *philo, size_t time_to_awake)
{
	while (1)
	{
		update_time(philo);
		if (philo->timmings.time_us > time_to_awake)
			return ;
		usleep(1);
	}
}
