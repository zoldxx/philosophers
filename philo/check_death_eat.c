/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death_eat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 19:52:32 by dberreby          #+#    #+#             */
/*   Updated: 2023/07/11 17:34:56 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_die(t_data *data) // tourne en boucle dans le main
{
	int	i;
	int	full_meal;

	i = 0;
	full_meal = 0;
	while (!check_death(data)) // on sort de la boucle si qqn est mort
	{
		while (i < data->nb_philo)
		{
			if (!check_full_meal(&data->phi[i])) // si le philo n'a pas mangé tous ses repas,
			{
				if (norm(data, i)) // si le philo n'a pas mangé depuis trop longtemps,
				{
					ft_death(data, i); // on informe la structure globale que qqn est mort et on sort de la boucle
					break ;
				}
			}
			i++;
		}
		i = 0;
		if (check_all_phi_meal(data))
			return (1);
	}
	return (1);
}

int	norm(t_data *data, int i) // dans cette fonction, on regarde si un philo est censé mourir ou non
{
	int	k;

	k = 0;
	pthread_mutex_lock(&data->meal);
	if (get_current_time(&data->phi[i])
		- data->phi[i].last_meal > data->phi[i].die_time) // si le philo n'a pas mangé depuis plus longtemps que die time, il meurt
		k = 1;
	else
		k = 0;
	pthread_mutex_unlock(&data->meal);
	return (k);
}

void	ft_death(t_data *data, int i) // se déclenche si qqn meurt
{
	pthread_mutex_lock(&data->dead);
	data->philo_is_dead = 1; // la variable qui permet d'informer tout le monde que qqn est mort
	pthread_mutex_unlock(&data->dead);
	pthread_mutex_lock(&(data->print));
	printf("%lld %d died\n", get_current_time(&data->phi[i]), // on affiche le message sans utiliser la fonction message car celle-ci ne marche que si personne n'est mort
		data->phi[i].num_philo);
	pthread_mutex_unlock(&(data->print));
}

int	check_all_phi_meal(t_data *data) // vérifie si tous les philos ont mangé tous leurs repas
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (!check_full_meal(&data->phi[i]))
			break ;
		i++;
	}
	if (i == data->nb_philo) // si chaque philo a mangé tous ses repas, on retourne 1 pour arreter le programme
		return (1);
	return (0);
}

int	check_full_meal(t_phi *phi) // check si un philo a mangé tous ses repas ou pas encore
{
	int	i;

	i = 0;
	pthread_mutex_lock(&phi->all->meal);
	if (phi->max_meal == -1) // si l'utilisateur n'a pas rentré l'argument optionnel, les philos n'auront jamais mangé tous leurs repas
		i = 0;
	else if (phi->max_meal > 0 && phi->nb_meal < phi->max_meal)
		i = 0;
	else
	{
		phi->last_meal = 0;
		i = 1;
	}
	pthread_mutex_unlock(&phi->all->meal);
	return (i);
}
