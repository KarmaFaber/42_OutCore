/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 13:24:55 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:34:11 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

/**
 * @brief Finds the zone containing the given pointer.
 *
 * @param ptr Pointer to locate within the zones.
 * @param prev_out Pointer to store the previous zone, if found.
 * @return Pointer to the containing zone, or NULL if not found.
 */
static t_zone	*find_zone_by_ptr(void *ptr, t_zone **prev_out)
{
	t_zone	*curr;
	t_zone	*prev;
	char	*start;
	char	*end;

	curr = g_zones;
	prev = NULL;
	if (ptr == NULL)
		return (NULL);
	while (curr != NULL)
	{
		start = (char *)curr;
		end = start + curr->total_size;
		if ((char *)ptr >= start && (char *)ptr < end)
		{
			if (prev_out)
				*prev_out = prev;
			return (curr);
		}
		prev = curr;
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Finds the block whose aligned payload matches the given pointer.
 *
 * @param zone Zone in which to search for the block.
 * @param ptr Pointer to the allocated payload.
 * @return Pointer to the matching block, or NULL if not found.
 */
static t_block	*find_block_by_ptr(t_zone *zone, void *ptr)
{
	t_block	*block;
	void	*payload;

	if (!zone || zone->total_size <= sizeof(t_zone) + sizeof(t_block))
		return (NULL);
	block = (t_block *)((char *)zone + sizeof(t_zone));
	while (1)
	{
		payload = (void *)ALIGN_16((uintptr_t)((char *)block
					+ sizeof(t_block)));
		if (payload == ptr)
			return (block);
		if (block->next_offset == 0)
			break ;
		block = (t_block *)((char *)zone + block->next_offset);
	}
	return (NULL);
}

/**
 * @brief Removes a large zone from the global list and unmaps it.
 *
 * @param zone Large zone to release.
 * @param prev Previous zone in the global list.
 */
static void	free_large_zone(t_zone *zone, t_zone *prev)
{
	if (prev == NULL)
		g_zones = zone->next;
	else
		prev->next = zone->next;
	(void)munmap(zone, zone->total_size);
}

/**
 * @brief Releases the memory associated with the given pointer.
 *
 * @param ptr Pointer to the memory to free.
 */
void	ft_free(void *ptr)
{
	t_zone	*zone;
	t_zone	*prev;
	t_block	*block;

	if (malloc_lock() != 0)
		return ;
	prev = NULL;
	zone = find_zone_by_ptr(ptr, &prev);
	if (zone != NULL)
	{
		if (zone->group == LARGE)
			free_large_zone(zone, prev);
		else
		{
			block = find_block_by_ptr(zone, ptr);
			if (block != NULL && block->is_free == 0)
			{
				block->is_free = 1;
				zone->free_size += block->size;
			}
		}
	}
	(void)malloc_unlock();
}

/**
 * @brief Releases allocated memory by calling ft_free.
 *
 * @param ptr Pointer to the memory to free.
 */
void	free(void *ptr)
{
	ft_free(ptr);
}
