/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 23:04:42 by pfrances          #+#    #+#             */
/*   Updated: 2022/12/13 18:31:09 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	release_fork(t_philo *philo, t_wich_forks forks_to_release)
{
	if (forks_to_release == RIGHT || forks_to_release == BOTH)
		pthread_mutex_unlock(philo->right_forks);
	if (forks_to_release == LEFT || forks_to_release == BOTH)
		pthread_mutex_unlock(philo->left_forks);
	return (false);
}

bool	take_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_forks);
		if (writing_logs(philo, TAKE_FORK_MSG) == false)
			return (release_fork(philo, LEFT));
		pthread_mutex_lock(philo->right_forks);
		if (writing_logs(philo, TAKE_FORK_MSG) == false)
			return (release_fork(philo, BOTH));
	}
	else
	{
		pthread_mutex_lock(philo->right_forks);
		if (writing_logs(philo, TAKE_FORK_MSG) == false)
			return (release_fork(philo, RIGHT));
		pthread_mutex_lock(philo->left_forks);
		if (writing_logs(philo, TAKE_FORK_MSG) == false)
			return (release_fork(philo, BOTH));
	}
	return (true);
}

void	update_eaten_meals(t_philo *philo)
{
	if (philo->meals_to_eat != SIZE_MAX)
	{
		pthread_mutex_lock(&philo->info->decrease_total_meals);
		if (philo->info->total_meals != 0
			&& philo->nbr_ate_meals < philo->meals_to_eat)
		philo->info->total_meals--;
		pthread_mutex_unlock(&philo->info->decrease_total_meals);
	}
	philo->nbr_ate_meals++;
}

bool	take_forks_and_eat(t_philo *philo)
{
	if (take_forks(philo) == false)
		return (false);
	philo->timmings.last_meal_timming = philo->timmings.time_us;
	if (writing_logs(philo, EAT_MSG) == false)
		return (release_fork(philo, BOTH));
	if (ZURUI)
		sleep_until(philo, philo->timmings.start_delay \
			+ philo->nbr_ate_meals * (philo->timmings.time_to_one_circuit) \
			+ philo->timmings.time_to_eat);
	else
		sleep_until(philo,
			philo->timmings.last_meal_timming + philo->timmings.time_to_eat);
	update_eaten_meals(philo);
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_forks);
		pthread_mutex_unlock(philo->left_forks);
	}
	else
	{
		pthread_mutex_unlock(philo->left_forks);
		pthread_mutex_unlock(philo->right_forks);
	}
	return (true);
}

bool	eat(t_philo *philo)
{
	while (1)
	{
		if (philo->info->can_continue == false || philo->info->nb_philo < 2)
			return (false);
		pthread_mutex_lock(philo->check_forks);
		if (philo->left_philo->state != IS_EATING
			&& philo->right_philo->state != IS_EATING)
		{
			philo->state = IS_EATING;
			pthread_mutex_unlock(philo->check_forks);
			if (take_forks_and_eat(philo) == false)
				return (false);
			break ;
		}
		pthread_mutex_unlock(philo->check_forks);
		usleep(1);
	}
	return (true);
}
