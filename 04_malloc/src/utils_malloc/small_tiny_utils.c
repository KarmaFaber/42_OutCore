/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_tiny_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:07:16 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:14:27 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

/**
 * @brief Finds a free block large enough to satisfy the allocation.
 *
 * @param zone Zone in which to search for a free block.
 * @param needed Required allocation size.
 * @return Pointer to the matching block, or NULL if none is found.
 */
t_block	*find_free_block(t_zone *zone, size_t needed)
{
	t_block	*block;
	size_t	payload_offset;
	size_t	block_end;

	if (!zone || zone->total_size <= sizeof(t_zone) + sizeof(t_block))
		return (NULL);
	block = (t_block *)((char *)zone + sizeof(t_zone));
	while (1)
	{
		payload_offset = ALIGN_16((size_t)((char *)block - (char *)zone)
				+ sizeof(t_block));
		block_end = zone->total_size;
		if (block->next_offset != 0)
			block_end = block->next_offset;
		if (block->is_free && block->size >= needed
			&& zone->free_size >= block->size
			&& block_end >= payload_offset
			&& block->size <= block_end - payload_offset)
			return (block);
		if (block->next_offset == 0)
			break ;
		block = (t_block *)((char *)zone + block->next_offset);
	}
	return (NULL);
}

/**
 * @brief Checks whether a free block can be split safely.
 *
 * @param zone Zone containing the block.
 * @param block Block to split.
 * @param needed Required allocation size.
 * @param split_offset Pointer to store the split offset.
 * @return 1 if the block can be split, 0 otherwise.
 */
static int	can_split_block(t_zone *zone, t_block *block, size_t needed,
		size_t *split_offset)
{
	size_t	payload_offset;
	size_t	block_end;
	size_t	split_payload_offset;

	payload_offset = ALIGN_16((size_t)((char *)block - (char *)zone)
			+ sizeof(t_block));
	block_end = zone->total_size;
	if (block->next_offset != 0)
		block_end = block->next_offset;
	if (payload_offset > SIZE_MAX - 15
		|| needed > SIZE_MAX - payload_offset - 15)
		return (0);
	*split_offset = ALIGN_16(payload_offset + needed);
	if (block_end < sizeof(t_block)
		|| *split_offset > UINT32_MAX
		|| *split_offset > block_end - sizeof(t_block))
		return (0);
	split_payload_offset = ALIGN_16(*split_offset + sizeof(t_block));
	if (split_payload_offset > block_end
		|| block_end - split_payload_offset < 16)
		return (0);
	return (1);
}

/**
 * @brief Updates the previous offset of the next block after a split.
 *
 * @param zone Zone containing the blocks.
 * @param old_next Offset of the next block before splitting.
 * @param split_offset Offset of the newly created block.
 */
static void	update_next_prev(t_zone *zone, size_t old_next,
		size_t split_offset)
{
	t_block	*next;

	if (old_next == 0)
		return ;
	next = (t_block *)((char *)zone + old_next);
	next->prev_offset = (uint32_t)split_offset;
}

/**
 * @brief Splits a block and updates the zone and block metadata.
 *
 * @param zone Zone containing the block.
 * @param block Block to split.
 * @param needed Size required for the allocation.
 * @param split_offset Offset where the new free block starts.
 */
static void	split_block(t_zone *zone, t_block *block, size_t needed,
		size_t split_offset)
{
	t_block	*split;
	size_t	free_size;
	size_t	old_next;

	old_next = block->next_offset;
	free_size = zone->total_size
		- ALIGN_16(split_offset + sizeof(t_block));
	if (old_next != 0)
		free_size -= zone->total_size - old_next;
	split = (t_block *)((char *)zone + split_offset);
	split->size = free_size;
	split->is_free = 1;
	split->next_offset = (uint32_t)old_next;
	split->prev_offset = (uint32_t)((char *)block - (char *)zone);
	update_next_prev(zone, old_next, split_offset);
	zone->free_size -= block->size - free_size;
	block->size = needed;
	block->is_free = 0;
	block->next_offset = (uint32_t)split_offset;
}

/**
 * @brief Allocates memory from a free block within an existing zone.
 *
 * @param zone Zone from which to allocate memory.
 * @param needed Required allocation size.
 * @return Pointer to the allocated payload, or NULL if no block is found.
 */
void	*allocate_from_zone(t_zone *zone, size_t needed)
{
	t_block	*block;
	size_t	payload_offset;
	size_t	split_offset;

	block = find_free_block(zone, needed);
	if (block == NULL)
		return (NULL);
	payload_offset = ALIGN_16((size_t)((char *)block - (char *)zone)
			+ sizeof(t_block));
	if (!can_split_block(zone, block, needed, &split_offset))
	{
		zone->free_size -= block->size;
		block->is_free = 0;
		return ((void *)((char *)zone + payload_offset));
	}
	split_block(zone, block, needed, split_offset);
	return ((void *)((char *)zone + payload_offset));
}
