/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:08:57 by dberreby          #+#    #+#             */
/*   Updated: 2023/07/11 16:56:31 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_exit(t_data *data) // fonction pour free et destroy tout ce qu'il faut a la fin
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->mutex[i]);
		i++;
	}
	free(data->mutex);
	free(data->phi);
	pthread_mutex_destroy(&(data->print));
	pthread_mutex_destroy(&(data->dead));
	pthread_mutex_destroy(&(data->meal));
}

int	init_thread(t_data *data) // initialiser les threads, je lance tous les pairs puis tous les impairs pour être Sûr qu'ils puissent tous agir Dès leur création
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&(data->phi[i].thread), NULL, thread_routine,
				&(data->phi[i])))
		{
			free_exit(data);
			return (0);
		}
		i += 2;
	}
	i = 1;
	while (i < data->nb_philo)
	{
		if (pthread_create(&(data->phi[i].thread), NULL, thread_routine,
				&(data->phi[i])))
		{
			free_exit(data);
			return (0);
		}
		i += 2;
	}
	return (1);
}

int	main(int ac, char **av) // main qui initialise tout, puis checkdie en boucle, puis pthreadjoin pour laisser les thread se finir, puis free/destroy tout
{
	t_data	data;
	int		i;

	i = 0;
	if (ac == 5 || ac == 6)
	{
		data.start_time = get_start_time(&data);
		if (initialisation(ac, av, &data) == 0)
			return (0);
		if (init_philo_mutex(&data) == 0)
			return (0);
		init_philo(&data);
		if (init_thread(&data) == 0)
			return (0);
		while (!check_die(&data)) // fonction qui tourne en boucle dans le thread #1 ( = le main, celui créé par défaut) pour verifier que personne n'est mort
			;					// on sort de la boucle quand un philo meurt ou que tout le monde a mangé le nombre de fois voulu
		while (i < data.nb_philo)
		{
			pthread_join(data.phi[i].thread, NULL); // permets d'attendre que tous les threads aient finis leur routine avant de quitter le programme
			i++;
		}
		free_exit(&data); // free et destroy tout
	}
	return (0);
}
