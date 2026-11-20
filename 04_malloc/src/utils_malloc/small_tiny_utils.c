/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_tiny_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:07:16 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 15:07:46 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

t_zone	*find_zone_for_alloc(t_heap_group group, size_t needed_size)
{
	t_zone	*curr;

	curr = g_zones;
	while (curr != NULL)
	{
		if (curr->group == group && curr->free_size >= needed_size)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

t_block	*find_free_block(t_zone *zone, size_t needed)
{
	t_block	*block;

	if (!zone || zone->total_size <= sizeof(t_zone) + sizeof(t_block))
		return (NULL);
	block = (t_block *)((char *)zone + sizeof(t_zone));
	while (1)
	{
		if (block->is_free && block->size >= needed)
			return (block);
		if (block->next_offset == 0)
			break ;
		block = (t_block *)((char *)zone + block->next_offset);
	}
	return (NULL);
}
