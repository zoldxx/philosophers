/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:21:20 by dberreby          #+#    #+#             */
/*   Updated: 2023/07/11 17:11:01 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	initialisation(int ac, char **av, t_data *data) // mets dans la structure globale toutes les valeurs rentrées par l'utilisateur
{
	data->philo_is_dead = 0;
	data->meal_finish = 0;
	data->nb_philo = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	if (data->nb_philo < 1 || data->die_time < 1 || data->eat_time < 1
		|| data->sleep_time < 1) // si qqch < 1, le programme ne peut pas fonctionner
		return (0);
	if (ac == 6) // si l'utilisateur rentre l'argument optionnel
	{
		data->max_meal = ft_atoi(av[5]);
		if (data->max_meal < 1)
			return (0);
	}
	else // si pas d'arguments optionnels, on le met a -1 pour faciliter la gestion des repas plus tard
	{
		data->max_meal = -1;
	}
	if (data->nb_philo == 1) // si 1 seul philo, il se passera tout le temps la meme chose (voir en dessous)
	{
		solo(data);
		return (0);
	}
	return (1);
}

void	solo(t_data *data)
{
	printf("0 1 has taken a fork\n");
	usleep(data->die_time * 1000);
	printf("%d 1 died\n", (data->die_time + 1));
}

int	init_philo_mutex(t_data *data) // init les tableaux de structure et les mutex
{
	int	i;

	i = 0;
	data->phi = malloc(sizeof(t_phi) * data->nb_philo); // crée un tableau de structure t_phi de la taille du nombre de philo 
	if (!data->phi)										// (donc une structure t_phi par philo)
		return (0);
	data->mutex = malloc(sizeof(pthread_mutex_t) * data->nb_philo); // crée un tableau de mutex de la taille du nombre de philo 
	if (!data->mutex)												// (donc un mutex par philo)
		return (0);
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&(data->mutex[i]), NULL)) // initialise chaque mutex de fourchette
			return (0);
		i++;
	}
	if (pthread_mutex_init(&(data->print), NULL)) // init le mutex qui permet de print
		return (0);
	if (pthread_mutex_init(&(data->dead), NULL)) // init le mutex qui permet de checker que qqn est mort
		return (0);
	if (pthread_mutex_init(&(data->meal), NULL)) // init le mutex qui permet de checker le nombre de repas d'un philo
		return (0);
	link_philo(data);
	return (1);
}

void	link_philo(t_data *data) // relie chaque philo à ses fourchettes, donc relie chaque philo à 2 mutex du tableau de mutex créé avant
{
	int	i;

	i = 0;
	while (i < data->nb_philo - 1)
	{
		data->phi[i].lf = &data->mutex[i];
		data->phi[i].rf = &data->mutex[i + 1];
		i++;
	}
	data->phi[i].lf = &data->mutex[i];
	data->phi[i].rf = &data->mutex[0];
}

void	init_philo(t_data *data) // IMPORTANT : mets dans chaque philo une copie de toutes les données de la structure globale
{								// permets à chaque philo de regarder dans sa propre structure si il a besoin d'une donnée (die time, sleep time...)
	int	i;						// ainsi, pas besoin de lock/unlock pour accéder a une variable de la structure globale car chaque philo a sa propre copie
								// de cette variable dans sa structure
	i = 0;
	while (i < data->nb_philo)
	{
		data->phi[i].num_philo = i + 1;
		data->phi[i].last_meal = 0;
		data->phi[i].nb_meal = 0;
		data->phi[i].all = data;
		data->phi[i].nb_philo = data->nb_philo;
		data->phi[i].die_time = data->die_time;
		data->phi[i].eat_time = data->eat_time;
		data->phi[i].sleep_time = data->sleep_time;
		data->phi[i].max_meal = data->max_meal;
		data->phi[i].start_time = data->start_time;
		i++;
	}
}
