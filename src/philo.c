/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pracksaw <pracksaw@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:23:13 by pracksaw          #+#    #+#             */
/*   Updated: 2025/02/04 23:22:31 by pracksaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

// UTILS
// int	ft_atoi(const char *str)
// {
// 	long int	n;
// 	int			sign;
// 	n = 0;
// 	sign = 1;
// 	while ((*str <= 13 && *str >= 9) || *str == 32)
// 		str++;
// 	if (*str == '-')
// 		return (-1);
// 	else if (*str == '+')
// 		str++;
// 	while (*str)
// 	{
// 		if (*str >= '0' && *str <= '9')
// 			n = n * 10 + (*str++ - '0');
// 		else
// 			return (-1);
// 	}
// 	return ((int)(n * sign));
// }
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

// INIT
int	init_mutex(t_rules *rules)
{
	int	i;

	i = rules->nb_philo;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(rules->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(rules->writing), NULL))
		return (1);
	if (pthread_mutex_init(&(rules->meal_check), NULL))
		return (1);
	return (0);
}

int	init_philos(t_rules *rules)
{
	int	i;

	i = rules->nb_philo;
	while (--i >= 0)
	{
		rules->philosophers[i].id = i;
		rules->philosophers[i].x_ate = 0;
		rules->philosophers[i].left_fork_id = i;
		rules->philosophers[i].right_fork_id = (i + 1) % rules->nb_philo;
		rules->philosophers[i].t_last_meal = 0;
		rules->philosophers[i].rules = rules;
	}
	return (0);
}

int	init_all(t_rules *rules, char **argv)
{
	rules->nb_philo = ft_atoi(argv[1]);
	rules->time_death = ft_atoi(argv[2]);
	rules->time_eat = ft_atoi(argv[3]);
	rules->time_sleep = ft_atoi(argv[4]);
	rules->all_ate = 0;
	rules->dieded = 0;
	if (rules->nb_philo < 1 || rules->time_death < 0 || rules->time_eat < 0
		|| rules->time_sleep < 0 || rules->nb_philo > MAX_P)
		return (1);
	if (argv[5])
	{
		rules->nb_eat = ft_atoi(argv[5]);
		if (rules->nb_eat <= 0)
			return (1);
	}
	else
		rules->nb_eat = -1;
	if (init_mutex(rules))
		return (2);
	init_philos(rules);
	return (0);
}

// err_handling
int	write_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(2, YEL "Error: " RE, 14);
	write(2, str, len);
	write(2, "\n", 1);
	return (1);
}

int	error_manager(int error)
{
	if (error == 1)
		return (write_error(RED "One or more arguments are invalid." RE));
	if (error == 2)
		return (write_error(RED "Mutex initialization failed." RE));
	return (1);
}

// LAUNCH
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

//==========================================
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

void	death_checker(t_rules *r, t_philo *p)
{
	int	i;
	int	j;
	int	all_ate;

	while (1)
	{
		i = -1;
		pthread_mutex_lock(&(r->meal_check));
		if (r->all_ate)
		{
			pthread_mutex_unlock(&(r->meal_check));
			break ;
		}
		pthread_mutex_unlock(&(r->meal_check));
		while (++i < r->nb_philo)
		{
			pthread_mutex_lock(&(r->meal_check));
			if (time_diff(p[i].t_last_meal, timestamp()) > r->time_death)
			{
				action_print(r, i, RED "died" RE);
				r->dieded = 1;
				pthread_mutex_unlock(&(r->meal_check));
				return ;
			}
			if (r->nb_eat != -1 && i == r->nb_philo - 1)
			{
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
					pthread_mutex_unlock(&(r->meal_check));
					return ;
				}
			}
			pthread_mutex_unlock(&(r->meal_check));
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
