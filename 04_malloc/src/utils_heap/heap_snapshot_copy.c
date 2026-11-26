/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_snapshot_copy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:29:51 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/heap_snapshot.h"

/**
 * @brief Copies block metadata and a payload preview into a snapshot.
 *
 * @param dst Destination snapshot block.
 * @param zone Zone containing the block.
 * @param block Block whose data is copied.
 */
static void	copy_block(t_snapshot_block *dst, t_zone *zone, t_block *block)
{
	unsigned char	*payload;
	size_t			i;

	dst->payload = ALIGN_16((uintptr_t)((char *)block + sizeof(t_block)));
	dst->size = block->size;
	dst->is_free = block->is_free;
	dst->next_offset = block->next_offset;
	dst->prev_offset = block->prev_offset;
	dst->preview_size = 0;
	if (dst->is_free || dst->size == 0)
		return ;
	dst->preview_size = dst->size;
	if (dst->preview_size > sizeof(dst->preview))
		dst->preview_size = sizeof(dst->preview);
	payload = (unsigned char *)dst->payload;
	i = 0;
	while (i < dst->preview_size
		&& (uintptr_t)(payload + i) < (uintptr_t)zone + zone->total_size)
	{
		dst->preview[i] = payload[i];
		i++;
	}
	dst->preview_size = i;
}

/**
 * @brief Copies zone metadata into the snapshot.
 *
 * @param dst Destination snapshot zone.
 * @param snapshot Heap snapshot containing the zones.
 * @param zone Zone whose metadata is copied.
 * @param index Index of the zone in the snapshot.
 */
static void	set_zone_metadata(t_snapshot_zone *dst, t_heap_snapshot *snapshot,
		t_zone *zone, size_t index)
{
	dst->group = zone->group;
	dst->address = (uintptr_t)zone;
	dst->next = (uintptr_t)zone->next;
	dst->total_size = zone->total_size;
	dst->free_size = zone->free_size;
	if (index == 0)
		dst->first_block = 0;
	else
		dst->first_block = snapshot->zones[index - 1].first_block
			+ snapshot->zones[index - 1].block_count;
}

/**
 * @brief Copies zone metadata and its blocks into the heap snapshot.
 *
 * @param snapshot Heap snapshot receiving the copied data.
 * @param zone Zone whose data is copied.
 * @param index Index of the zone in the snapshot.
 * @param block_count Number of blocks in the zone.
 */
static void	copy_zone(t_heap_snapshot *snapshot, t_zone *zone,
		size_t index, size_t block_count)
{
	t_snapshot_zone	*dst;
	t_block			*block;
	size_t			offset;
	size_t			i;

	dst = &snapshot->zones[index];
	set_zone_metadata(dst, snapshot, zone, index);
	dst->block_count = block_count;
	offset = sizeof(t_zone);
	i = 0;
	while (i < block_count)
	{
		block = (t_block *)((char *)zone + offset);
		copy_block(&snapshot->blocks[dst->first_block + i], zone, block);
		offset = block->next_offset;
		i++;
	}
}

/**
 * @brief Copies all zones and their blocks into the heap snapshot.
 *
 * @param snapshot Heap snapshot receiving the copied data.
 * @param counts Array containing the block count for each zone.
 */
void	snapshot_copy_heap(t_heap_snapshot *snapshot, size_t counts[MAX_ZONES])
{
	t_zone	*zone;
	size_t	i;

	zone = g_zones;
	i = 0;
	while (i < snapshot->zone_count)
	{
		copy_zone(snapshot, zone, i, counts[i]);
		zone = zone->next;
		i++;
	}
}

/**
 * @brief Sorts snapshot zones by their memory addresses.
 *
 * @param snapshot Heap snapshot whose zones are sorted.
 */
void	snapshot_sort_zones(t_heap_snapshot *snapshot)
{
	t_snapshot_zone	tmp;
	size_t			i;
	size_t			j;

	i = 0;
	while (i < snapshot->zone_count)
	{
		j = i + 1;
		while (j < snapshot->zone_count)
		{
			if (snapshot->zones[j].address < snapshot->zones[i].address)
			{
				tmp = snapshot->zones[i];
				snapshot->zones[i] = snapshot->zones[j];
				snapshot->zones[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
