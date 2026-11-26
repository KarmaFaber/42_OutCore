/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_snapshot.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:32:14 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/heap_snapshot.h"

/**
 * @brief Fills the snapshot with a consistent copy of the heap metadata.
 *
 * @param snapshot Snapshot to populate.
 * @param block_capacity Maximum number of blocks the snapshot can hold.
 * @param mapping_size Allocated snapshot mapping size.
 * @param counts Array containing the block count for each zone.
 * @return 0 on success, 1 if capacity must be increased, or -1 on error.
 */
static int	fill_snapshot(t_heap_snapshot *snapshot, size_t block_capacity,
		size_t mapping_size, size_t counts[MAX_ZONES])
{
	size_t	zone_count;
	size_t	block_count;
	int		failed;
	int		retry;

	if (malloc_lock() != 0)
		return (-1);
	failed = snapshot_count_heap(&zone_count, &block_count, counts) != 0;
	retry = !failed && block_count > block_capacity;
	if (failed)
		errno = EINVAL;
	if (!failed && !retry)
	{
		snapshot->mapping_size = mapping_size;
		snapshot->zone_count = zone_count;
		snapshot->block_count = block_count;
		snapshot_copy_heap(snapshot, counts);
	}
	if (malloc_unlock() != 0)
		return (-1);
	if (failed)
		return (-1);
	return (retry);
}

/**
 * @brief Maps memory and fills it with a heap snapshot.
 *
 * @param block_capacity Maximum number of blocks the snapshot can hold.
 * @param mapping_size Required mapping size.
 * @param counts Array containing the block count for each zone.
 * @param retry Pointer indicating whether another allocation attempt is needed.
 * @return Pointer to the populated snapshot, or NULL on failure.
 */
static t_heap_snapshot	*map_and_fill(size_t block_capacity,
		size_t mapping_size, size_t counts[MAX_ZONES], int *retry)
{
	t_heap_snapshot	*snapshot;
	int				result;

	*retry = 0;
	snapshot = mmap(NULL, mapping_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (snapshot == MAP_FAILED)
		return (NULL);
	result = fill_snapshot(snapshot, block_capacity, mapping_size, counts);
	*retry = (result > 0);
	if (result != 0)
	{
		(void)munmap(snapshot, mapping_size);
		return (NULL);
	}
	return (snapshot);
}

/**
 * @brief Creates a sorted snapshot of the current heap state.
 *
 * @param None.
 * @return Pointer to the created snapshot, or NULL on failure.
 */
t_heap_snapshot	*heap_snapshot_create(void)
{
	t_heap_snapshot	*snapshot;
	size_t			counts[MAX_ZONES];
	size_t			block_capacity;
	size_t			mapping_size;
	int				retry;

	while (1)
	{
		if (snapshot_count_capacity(&block_capacity,
				&mapping_size, counts) != 0)
			return (NULL);
		snapshot = map_and_fill(block_capacity, mapping_size, counts, &retry);
		if (snapshot)
		{
			snapshot_sort_zones(snapshot);
			return (snapshot);
		}
		if (!retry)
			return (NULL);
	}
}

/**
 * @brief Releases the memory mapping associated with a heap snapshot.
 *
 * @param snapshot Snapshot to destroy.
 */
void	heap_snapshot_destroy(t_heap_snapshot *snapshot)
{
	if (snapshot)
		(void)munmap(snapshot, snapshot->mapping_size);
}
