/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writing_logs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:26:53 by pfrances          #+#    #+#             */
/*   Updated: 2022/11/20 16:38:39 by pfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	writing_logs(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->info->writing_logs_access);
	if (philo->info->can_continue == false)
	{
		pthread_mutex_unlock(&philo->info->writing_logs_access);
		return (false);
	}
	update_time(philo);
	printf("%ld %ld %s", philo->timmings.time_us / 1000, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->info->writing_logs_access);
	return (true);
}
