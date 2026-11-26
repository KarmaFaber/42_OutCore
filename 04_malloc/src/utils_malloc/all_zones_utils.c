/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_zones_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:11:45 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

/**
 * @brief Links a new zone to the global zone list.
 *
 * @param ctx Zone initialization context.
 * @param group Heap group assigned to the zone.
 * @param free_size Available free space in the zone.
 */
void	link_new_zone(t_zone_ctx *ctx, t_heap_group group, size_t free_size)
{
	ctx->zone->group = group;
	ctx->zone->total_size = ctx->total_size;
	ctx->zone->free_size = free_size;
	ctx->zone->next = g_zones;
	g_zones = ctx->zone;
}

/**
 * @brief Initializes the first block of a zone.
 *
 * @param ctx Zone initialization context.
 * @param is_free Block free status.
 */
void	init_first_block(t_zone_ctx *ctx, int is_free)
{
	ctx->block = (t_block *)((char *)ctx->zone + sizeof(t_zone));
	ctx->block->size = ctx->aligned_size;
	ctx->block->is_free = is_free;
	ctx->block->next_offset = 0;
	ctx->block->prev_offset = 0;
}

/**
 * @brief Initializes the next free block in the zone.
 *
 * @param ctx Zone initialization context.
 * @return Size of the next free block, or 0 if it cannot be created.
 */
static int	next_free_block_offset(t_zone_ctx *ctx, uint32_t *next_off)
{
	size_t		payload_offset;
	size_t		used_end;
	size_t		next_offset;

	payload_offset = ALIGN_16(sizeof(t_zone) + sizeof(t_block));
	if (ctx->aligned_size > SIZE_MAX - payload_offset)
		return (0);
	used_end = payload_offset + ctx->aligned_size;
	if (used_end > SIZE_MAX - 15)
		return (0);
	next_offset = ALIGN_16(used_end);
	if (next_offset > UINT32_MAX
		|| ctx->total_size < sizeof(t_block)
		|| next_offset > ctx->total_size - sizeof(t_block))
		return (0);
	*next_off = (uint32_t)next_offset;
	return (1);
}

/**
 * @brief Initializes the next free block within the zone.
 *
 * @param ctx Zone initialization context.
 * @return Size of the free block, or 0 if it cannot be initialized.
 */
size_t	init_next_free_block(t_zone_ctx *ctx)
{
	t_block		*next;
	size_t		payload_offset;
	size_t		free_size;
	uint32_t	next_off;

	if (!next_free_block_offset(ctx, &next_off))
		return (0);
	payload_offset = ALIGN_16((size_t)next_off + sizeof(t_block));
	if (payload_offset > ctx->total_size
		|| ctx->total_size - payload_offset < 16)
		return (0);
	free_size = ctx->total_size - payload_offset;
	next = (t_block *)((char *)ctx->zone + next_off);
	next->size = free_size;
	next->is_free = 1;
	next->next_offset = 0;
	next->prev_offset = (uint32_t)((char *)ctx->block - (char *)ctx->zone);
	ctx->block->next_offset = next_off;
	return (free_size);
}

/**
 * @brief Creates a new zone and initializes its first block.
 *
 * @param ctx Zone initialization context.
 * @param group Heap group assigned to the zone.
 * @return Aligned payload pointer, or NULL if mmap fails.
 */
void	*create_new_zone(t_zone_ctx *ctx, t_heap_group group)
{
	size_t	free_size;

	ctx->zone = (t_zone *)mmap(NULL, ctx->total_size,
			PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if ((void *)ctx->zone == MAP_FAILED)
		return (NULL);
	free_size = 0;
	init_first_block(ctx, 0);
	if (group != LARGE)
		free_size = init_next_free_block(ctx);
	link_new_zone(ctx, group, free_size);
	ctx->raw_payload = (uintptr_t)((char *)ctx->block + sizeof(t_block));
	ctx->payload = (void *)ALIGN_16(ctx->raw_payload);
	return (ctx->payload);
}
