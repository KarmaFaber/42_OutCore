/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sorted_zones.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:33:06 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

/**
 * @brief Sorts zone pointers by their memory addresses.
 *
 * @param list Array of zone pointers to sort.
 * @param count Number of zones in the array.
 */
static void	sort_zones(t_zone **list, int count)
{
	t_zone	*tmp;
	int		i;
	int		j;

	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if ((uintptr_t)list[j] < (uintptr_t)list[i])
			{
				tmp = list[i];
				list[i] = list[j];
				list[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Retrieves and sorts the global list of memory zones.
 *
 * @param list Array used to store the sorted zone pointers.
 * @return Number of zones collected, or -1 if an error occurs.
 */
int	get_sorted_zones(t_zone **list)
{
	t_zone	*curr;
	int		count;

	if (list == NULL)
		return (-1);
	if (malloc_lock() != 0)
		return (-1);
	count = 0;
	curr = g_zones;
	while (curr && count < MAX_ZONES)
	{
		list[count++] = curr;
		curr = curr->next;
	}
	sort_zones(list, count);
	if (malloc_unlock() != 0)
		return (-1);
	return (count);
}
