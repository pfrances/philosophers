/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 23:04:42 by pfrances          #+#    #+#             */
/*   Updated: 2022/10/22 00:21:11 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks_and_eat(t_philo *philo)
{
	philo->state = IS_EATING;
	pthread_mutex_unlock(philo->check_forks);
	pthread_mutex_lock(philo->left_forks);
	printf("%ld %ld has taken a fork\n", *philo->time_ms, philo->id);
	pthread_mutex_lock(philo->right_forks);
	printf("%ld %ld has taken a fork\n%ld %ld is eating\n",
		*philo->time_ms, philo->id, *philo->time_ms, philo->id);
	philo->last_meal = *philo->time_us;
	usleep(philo->rules->time_to_eat - *philo->time_us % 1000);
	philo->nbr_ate_meals++;
	pthread_mutex_unlock(philo->right_forks);
	pthread_mutex_unlock(philo->left_forks);
}

void	eat(t_philo *philo)
{
	while (1)
	{
		if (philo->can_continue == false)
			return ;
		pthread_mutex_lock(philo->check_forks);
		if (philo->left_philo->state != IS_EATING
			&& philo->right_philo->state != IS_EATING)
		{
			take_forks_and_eat(philo);
			return ;
		}
		pthread_mutex_unlock(philo->check_forks);
	}
}
