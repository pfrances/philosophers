/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writing_logs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:26:53 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/21 10:00:07 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

bool	writing_logs(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->info->writing_logs_access);
	if (philo->info->can_continue == false && ft_strncmp(msg, DIED, 6) != 0)
	{
		pthread_mutex_unlock(&philo->info->writing_logs_access);
		return (false);
	}
	update_time(philo);
	printf("%ld %ld %s", philo->timmings.time_us / 1000, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->info->writing_logs_access);
	return (true);
}
