/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pracksaw <pracksaw@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 23:47:20 by pracksaw          #+#    #+#             */
/*   Updated: 2025/02/04 23:47:31 by pracksaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	exit_launcher(t_rules *rules, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < rules->nb_philo)
		pthread_join(philos[i].thread_id, NULL);
	i = -1;
	while (++i < rules->nb_philo)
		pthread_mutex_destroy(&(rules->forks[i]));
	pthread_mutex_destroy(&(rules->writing));
}

void	acquire_forks(t_philo *philo)
{
	t_rules	*rules;
	int		first_fork;
	int		second_fork;

	rules = philo->rules;
	first_fork = philo->left_fork_id;
	second_fork = philo->right_fork_id;
	if (first_fork < second_fork)
	{
		pthread_mutex_lock(&(rules->forks[first_fork]));
		action_print(rules, philo->id, CYA "has taken a fork" RE);
		pthread_mutex_lock(&(rules->forks[second_fork]));
		action_print(rules, philo->id, BLU "has taken a fork" RE);
	}
	else
	{
		pthread_mutex_lock(&(rules->forks[second_fork]));
		action_print(rules, philo->id, BLU "has taken a fork" RE);
		pthread_mutex_lock(&(rules->forks[first_fork]));
		action_print(rules, philo->id, CYA "has taken a fork" RE);
	}
}

void	philo_eats(t_philo *philo)
{
	t_rules	*rules;
	int		first_fork;
	int		second_fork;

	rules = philo->rules;
	first_fork = philo->left_fork_id;
	second_fork = philo->right_fork_id;
	if (rules->nb_philo == 1)
	{
		pthread_mutex_lock(&(rules->forks[first_fork]));
		action_print(rules, philo->id, CYA "has taken a fork" RE);
		smart_sleep(rules->time_death * 4200, rules);
		pthread_mutex_unlock(&(rules->forks[first_fork]));
		return ;
	}
	acquire_forks(philo);
	pthread_mutex_lock(&(rules->meal_check));
	action_print(rules, philo->id, YEL "is eating" RE);
	philo->t_last_meal = timestamp();
	philo->x_ate++;
	pthread_mutex_unlock(&(rules->meal_check));
	smart_sleep(rules->time_eat, rules);
	pthread_mutex_unlock(&(rules->forks[second_fork]));
	pthread_mutex_unlock(&(rules->forks[first_fork]));
}

void	*p_thread(void *void_philo)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)void_philo;
	rules = philo->rules;
	if (philo->id % 2)
		usleep(42000);
	while (1)
	{
		pthread_mutex_lock(&(rules->meal_check));
		if (rules->dieded || rules->all_ate)
		{
			pthread_mutex_unlock(&(rules->meal_check));
			break ;
		}
		pthread_mutex_unlock(&(rules->meal_check));
		philo_eats(philo);
		action_print(rules, philo->id, MAG "is sleeping" RE);
		smart_sleep(rules->time_sleep, rules);
		action_print(rules, philo->id, GRE "is thinking" RE);
	}
	return (NULL);
}
