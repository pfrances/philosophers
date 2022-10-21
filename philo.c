/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:52:44 by pfrances          #+#    #+#             */
/*   Updated: 2022/10/22 00:14:20 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	do_sleep(t_philo *philo)
{
	if (philo->can_continue == false)
		return ;
	philo->state = IS_SLEEPING;
	printf("%ld %ld is sleeping\n", *philo->time_ms, philo->id);
	usleep(philo->rules->time_to_sleep - *philo->time_us % 1000);
}

void	do_think(t_philo *philo)
{
	if (philo->can_continue == false)
		return ;
	philo->state = IS_THINKING;
	printf("%ld %ld is thinking\n", *philo->time_ms, philo->id);
	while ((philo->last_meal > philo->left_philo->last_meal)
		|| (philo->last_meal > philo->right_philo->last_meal))
		usleep(500 - *philo->time_us % 500);
}

void	*philo_daily_life(void *arg)
{
	t_philo	*philo;
	size_t	i;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		do_think(philo);
	i = 0;
	while (philo->rules->nbr_of_meals == SIZE_MAX
		|| i < philo->rules->nbr_of_meals)
	{
		if (philo->can_continue == false)
			return (NULL);
		eat(philo);
		do_sleep(philo);
		do_think(philo);
		i++;
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
		while (i < config.rules.nb_philo)
		{
			if (pthread_create(&config.philos[i].thread_id, NULL,
					&philo_daily_life, &config.philos[i]) != 0)
				printf("Failed to create philo thread\n");
			i++;
		}
		i = 0;
		check_end(&config);
	}
	return (0);
}
