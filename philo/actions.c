/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 14:38:28 by dberreby          #+#    #+#             */
/*   Updated: 2023/07/11 17:26:42 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_death(t_data *data) // check rapidement si qqn est mort. la vraie vérification se fait dans le fichier check_death_eat.c
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->dead);
	i = data->philo_is_dead;
	pthread_mutex_unlock(&data->dead);
	return (i);
}

void	*thread_routine(void *philosoph) // routine suivie par chaque philo
{
	t_phi	*phi;

	phi = (t_phi *)philosoph; // copie de l'argument envoyé a la fonction car pthreadcreate ne permet d'envoyer qu'un void * en argument
	while (!check_death(phi->all) && !check_full_meal(phi)) // on ne lance pas la routine si qqn est mort ou si tout le monde a mangé
	{
		lock_forks(phi);
		message(phi, "has taken a fork");
		message(phi, "has taken a fork");
		message(phi, "is eating");
		pthread_mutex_lock(&phi->all->meal);
		phi->last_meal = get_current_time(phi);
		phi->nb_meal++;
		pthread_mutex_unlock(&phi->all->meal);
		ft_usleep(phi->eat_time, phi->all);
		unlock_forks(phi);
		message(phi, "is sleeping");
		ft_usleep(phi->sleep_time, phi->all);
		message(phi, "is thinking");
		thinking(phi);
	}
	return (NULL);
}

void	thinking(t_phi *phi) // si le nombre de philo est impair, permets d'attendre avant de relancer une routine pour etre sur que tout le monde est bien coordonné
{							 // et qu'il n'y ai pas un philo qui mange 2 fois alors que tout le monde n'a pas encore mangé
	if (phi->nb_philo % 2)
	{
		ft_usleep((phi->eat_time - phi->sleep_time + 5), phi->all);
	}
}

void	lock_forks(t_phi *phi) // permets d'éviter les deadlocks
{
	if (phi->num_philo % 2)
	{
		pthread_mutex_lock(phi->lf);
		pthread_mutex_lock(phi->rf);
	}
	else
	{
		pthread_mutex_lock(phi->rf);
		pthread_mutex_lock(phi->lf);
	}
}

void	unlock_forks(t_phi *phi) // permets d'éviter les deadlocks
{
	if (phi->num_philo % 2)
	{
		pthread_mutex_unlock(phi->rf);
		pthread_mutex_unlock(phi->lf);
	}
	else
	{
		pthread_mutex_unlock(phi->lf);
		pthread_mutex_unlock(phi->rf);
	}
}
