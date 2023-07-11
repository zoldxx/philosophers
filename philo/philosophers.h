/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 15:10:05 by dberreby          #+#    #+#             */
/*   Updated: 2023/07/11 17:37:02 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_phi
{
	int				nb_philo;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				max_meal;
	long long		start_time;

	int				num_philo;
	long			last_meal; // timestamp du dernier repas
	int				nb_meal;
	pthread_t		thread;
	pthread_mutex_t	*lf;
	pthread_mutex_t	*rf;
	struct s_data	*all; // pointeur vers structure globale
}					t_phi;

typedef struct s_data
{
	int				nb_philo;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				max_meal;
	int				meal_finish; // booleen pour savoir si tout le monde a assez mangé

	long long		start_time;
	long			current_time;

	int				philo_is_dead; // booleen pour savoir si qqn est mort

	pthread_mutex_t	meal;
	pthread_mutex_t	dead;
	pthread_mutex_t	print;
	pthread_mutex_t	*mutex; // tableau de mutex de la taille du nombre de philo
	t_phi			*phi;	// tableau de philo de la taille du nombre de philo
}					t_data;

int					ft_atoi(char *nptr);

int					initialisation(int ac, char **av, t_data *data);

int					init_philo_mutex(t_data *data);

void				init_philo(t_data *data);

long				get_start_time(t_data *data);

long long			get_current_time(t_phi *phi);

void				*thread_routine(void *philosoph);

void				ft_usleep(int i, t_data *data);

void				free_exit(t_data *data);

void				message(t_phi *phi, char *str);

void				solo(t_data *data);

int					check_die(t_data *data);

void				check_meal(t_data *data);

void				lock_forks(t_phi *phi);

void				unlock_forks(t_phi *phi);

int					check_death(t_data *data);

void				ft_death(t_data *data, int i);

int					check_full_meal(t_phi *phi);

int					check_all_phi_meal(t_data *data);

int					norm(t_data *data, int i);

void				thinking(t_phi *phi);

void				link_philo(t_data *data);

#endif
