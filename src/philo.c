/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pracksaw <pracksaw@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:23:13 by pracksaw          #+#    #+#             */
/*   Updated: 2025/02/04 23:46:59 by pracksaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

// 0= not all ate 1 = all ate
static int	check_all_ate(t_rules *r, t_philo *p)
{
	int	j;
	int	all_ate;

	all_ate = 1;
	j = 0;
	while (j < r->nb_philo)
	{
		if (p[j].x_ate < r->nb_eat)
		{
			all_ate = 0;
			break ;
		}
		j++;
	}
	if (all_ate)
	{
		r->all_ate = 1;
		return (1);
	}
	return (0);
}

// 0=No ded/ate 1 = ded
static int	check_philosophers(t_rules *r, t_philo *p)
{
	int	i;
	int	result;

	i = -1;
	while (++i < r->nb_philo)
	{
		pthread_mutex_lock(&(r->meal_check));
		if (time_diff(p[i].t_last_meal, timestamp()) > r->time_death)
		{
			action_print(r, i, RED "died" RE);
			r->dieded = 1;
			pthread_mutex_unlock(&(r->meal_check));
			return (1);
		}
		if (r->nb_eat != -1 && i == r->nb_philo - 1)
		{
			result = check_all_ate(r, p);
			if (result == 1)
				return (pthread_mutex_unlock(&(r->meal_check)), 2);
		}
		pthread_mutex_unlock(&(r->meal_check));
	}
	return (0);
}

// res 1 = ded res 2 = ate
void	death_checker(t_rules *r, t_philo *p)
{
	int	result;

	while (1)
	{
		pthread_mutex_lock(&(r->meal_check));
		if (r->all_ate)
		{
			pthread_mutex_unlock(&(r->meal_check));
			break ;
		}
		pthread_mutex_unlock(&(r->meal_check));
		result = check_philosophers(r, p);
		if (result == 1)
		{
			return ;
		}
		else if (result == 2)
		{
			break ;
		}
		usleep(100);
	}
}

int	launcher(t_rules *rules)
{
	int		i;
	t_philo	*phi;

	phi = rules->philosophers;
	pthread_mutex_lock(&(rules->meal_check));
	rules->first_timestamp = timestamp();
	i = 0;
	while (i < rules->nb_philo)
	{
		phi[i].t_last_meal = timestamp();
		if (pthread_create(&(phi[i].thread_id), NULL, p_thread, &(phi[i])))
		{
			pthread_mutex_unlock(&(rules->meal_check));
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&(rules->meal_check));
	death_checker(rules, rules->philosophers);
	exit_launcher(rules, phi);
	return (0);
}

// MAIN
int	main(int argc, char **argv)
{
	t_rules	rules;
	int		ret;

	if (argc != 5 && argc != 6)
		return (write_error(RED "Incorrect number of arguments." RE));
	ret = init_all(&rules, argv);
	if (ret)
		return (error_manager(ret));
	if (launcher(&rules))
		return (write_error(RED "Error during thread creation." RE));
	return (0);
}
