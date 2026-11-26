/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_snapshot_count.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:31:30 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/heap_snapshot.h"

/**
 * @brief Counts the blocks in a zone and validates their offsets.
 *
 * @param zone Zone whose blocks are counted.
 * @param block_count Pointer to store the number of blocks.
 * @return 0 on success, or -1 if the zone metadata is invalid.
 */
static int	count_zone_blocks(t_zone *zone, size_t *block_count)
{
	t_block	*block;
	size_t	offset;
	size_t	count;

	if (zone->total_size < sizeof(t_zone) + sizeof(t_block))
		return (-1);
	offset = sizeof(t_zone);
	count = 0;
	while (count < zone->total_size / sizeof(t_block))
	{
		if (offset > zone->total_size - sizeof(t_block))
			return (-1);
		block = (t_block *)((char *)zone + offset);
		count++;
		if (block->next_offset == 0)
		{
			*block_count = count;
			return (0);
		}
		if (block->next_offset <= offset
			|| block->next_offset > zone->total_size - sizeof(t_block))
			return (-1);
		offset = block->next_offset;
	}
	return (-1);
}

/**
 * @brief Counts heap zones and blocks and stores per-zone block counts.
 *
 * @param zone_count Pointer to store the number of zones.
 * @param block_count Pointer to store the total number of blocks.
 * @param counts Array to store the block count of each zone.
 * @return 0 on success, or -1 if counting fails or overflow occurs.
 */
int	snapshot_count_heap(size_t *zone_count, size_t *block_count,
		size_t counts[MAX_ZONES])
{
	t_zone	*zone;
	size_t	count;

	*zone_count = 0;
	*block_count = 0;
	zone = g_zones;
	while (zone && *zone_count < MAX_ZONES)
	{
		if (count_zone_blocks(zone, &count) != 0
			|| count > SIZE_MAX - *block_count)
			return (-1);
		counts[*zone_count] = count;
		*block_count += count;
		(*zone_count)++;
		zone = zone->next;
	}
	return (0);
}

/**
 * @brief Calculates the mapping size required for a heap snapshot.
 *
 * @param block_count Number of blocks to include in the snapshot.
 * @param mapping_size Pointer to store the required mapping size.
 * @return 0 on success, or -1 if the size calculation overflows.
 */
static int	get_mapping_size(size_t block_count, size_t *mapping_size)
{
	if (block_count > (SIZE_MAX - sizeof(t_heap_snapshot))
		/ sizeof(((t_heap_snapshot *)0)->blocks[0]))
		return (-1);
	*mapping_size = sizeof(t_heap_snapshot)
		+ block_count * sizeof(((t_heap_snapshot *)0)->blocks[0]);
	return (0);
}

/**
 * @brief Counts heap blocks and calculates the snapshot mapping capacity.
 *
 * @param block_capacity Pointer to store the total block capacity.
 * @param mapping_size Pointer to store the required mapping size.
 * @param counts Array to store the block count of each zone.
 * @return 0 on success, or -1 if counting or sizing fails.
 */
int	snapshot_count_capacity(size_t *block_capacity, size_t *mapping_size,
		size_t counts[MAX_ZONES])
{
	size_t	zone_count;
	size_t	block_count;
	int		failed;

	if (malloc_lock() != 0)
		return (-1);
	failed = snapshot_count_heap(&zone_count, &block_count, counts) != 0;
	if (malloc_unlock() != 0)
		return (-1);
	if (failed)
	{
		errno = EINVAL;
		return (-1);
	}
	*block_capacity = block_count;
	if (get_mapping_size(block_count, mapping_size) != 0)
	{
		errno = ENOMEM;
		return (-1);
	}
	return (0);
}
