/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pracksaw <pracksaw@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:13:50 by pracksaw          #+#    #+#             */
/*   Updated: 2025/02/04 20:25:12 by pracksaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdint.h>
# include <stddef.h>
# include <limits.h>

# define RED		"\033[0;31m"
# define GRE		"\033[0;32m"
# define YEL		"\033[0;33m"
# define BLU		"\033[0;34m"
# define MAG		"\033[0;35m"
# define CYA		"\033[0;36m"
# define GRA		"\033[0;37m"
# define LRE		"\033[0;91m"
# define BOLD		"\033[1m"
# define RE			"\033[0m"
# define MAX_P		200

typedef struct s_philo
{
	int					id;
	int					x_ate;
	int					left_fork_id;
	int					right_fork_id;
	long long			t_last_meal;
	struct s_rules		*rules;
	pthread_t			thread_id;
}						t_philo;

typedef struct s_rules
{
	int					nb_philo;
	int					time_death;
	int					time_eat;
	int					time_sleep;
	int					nb_eat;
	int					dieded;
	int					all_ate;
	long long			first_timestamp;
	pthread_mutex_t		meal_check;
	pthread_mutex_t		forks[MAX_P];
	pthread_mutex_t		writing;
	t_philo				philosophers[MAX_P];
}						t_rules;

// ----- error_manager.c -----

int						write_error(char *str);
int						error_manager(int error);

// ----- init.c -----

int						init_all(t_rules *rules, char **argv);

// ----- utils.c -----

int						ft_atoi(const char *str);
void					action_print(t_rules *rules, int id, char *string);
long long				timestamp(void);
long long				time_diff(long long past, long long curr);
void					smart_sleep(long long time, t_rules *rules);

// ----- launcher.c -----

int						launcher(t_rules *rules);
void					exit_launcher(t_rules *rules, t_philo *philos);
void					philo_eats(t_philo *philo);

#endif
