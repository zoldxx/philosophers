/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dberreby <dberreby@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 12:32:54 by dberreby          #+#    #+#             */
/*   Updated: 2023/07/11 17:19:41 by dberreby         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *nptr)
{
	int	i;
	int	r;
	int	s;

	i = 0;
	r = 0;
	s = 1;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			s *= -1;
		i++;
	}
	if (!(nptr[i] >= '0' && nptr[i] <= '9'))
		return (0);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		r = r * 10 + (nptr[i] - 48);
		i++;
	}
	if (nptr[i] != '\0')
		return (0);
	return (r * s);
}

long	get_start_time(t_data *data) // renvoie le timestamp de départ
{
	struct timeval	tmp;

	(void)data;
	gettimeofday(&tmp, NULL);
	return ((tmp.tv_sec * 1000) + (tmp.tv_usec / 1000));
}

long long	get_current_time(t_phi *phi) // renvoie le timestamp actuel. Comme chaque philo a une copie du timestamp de départ, pas besoin de regarder dans la structure globale
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	return (((tmp.tv_sec * 1000) + (tmp.tv_usec / 1000)) - phi->start_time);
}

void	ft_usleep(int i, t_data *data) // la boucle permet de checker toutes les 10 millisecondes que personne n'est mort car si quelqu'un meurt, on doit arreter d'attendre
{
	int	a;

	a = 0;
	while (a < i && !check_death(data))
	{
		usleep(10000);
		a += 10;
	}
}

void	message(t_phi *phi, char *str) // affiche les messages
{
	if (!check_death(phi->all)) // si un philo est mort, on ne doit pas afficher de messages
	{
		pthread_mutex_lock(&(phi->all->print));
		printf("%lld %d %s\n", get_current_time(phi), phi->num_philo, str);
		pthread_mutex_unlock(&(phi->all->print)); // le lock/unlock permet d'etre sur que chaque message s'affiche a tour de role, donc évite que les printf se mélangent
	}
}
