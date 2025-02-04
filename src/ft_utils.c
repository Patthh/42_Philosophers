/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pracksaw <pracksaw@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 23:41:10 by pracksaw          #+#    #+#             */
/*   Updated: 2025/02/04 23:43:09 by pracksaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_atoi(const char *str)
{
	long int	n;
	int			sign;

	n = 0;
	sign = 1;
	while ((*str <= 13 && *str >= 9) || *str == 32)
		str++;
	if (*str == '-')
		return (-1);
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			n = n * 10 + (*str++ - '0');
			if (n > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return ((int)(n * sign));
}

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

long long	time_diff(long long past, long long curr)
{
	return (curr - past);
}

void	smart_sleep(long long time, t_rules *rules)
{
	long long	start;
	int			is_dead;

	start = timestamp();
	while (1)
	{
		pthread_mutex_lock(&rules->meal_check);
		is_dead = rules->dieded;
		pthread_mutex_unlock(&rules->meal_check);
		if (is_dead || (time_diff(start, timestamp()) >= time))
			break ;
		usleep(100);
	}
}

void	action_print(t_rules *rules, int id, char *string)
{
	pthread_mutex_lock(&(rules->writing));
	if (!(rules->dieded))
	{
		printf("%lli ", timestamp() - rules->first_timestamp);
		printf("%i ", id + 1);
		printf("%s\n", string);
	}
	pthread_mutex_unlock(&(rules->writing));
	return ;
}
