/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_zones_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 15:17:42 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

int		calculate_pages_num (size_t size, int ref, int page_size)
{
	int	pages_num;
	
	pages_num = 0;
	pages_num = (sizeof(t_zone) + ref * (sizeof(t_block) + size) + page_size - 1) / page_size;
	
	return (pages_num);
}

void	link_new_zone(t_zone_ctx *ctx, t_heap_group group, size_t free_size)
{
	ctx->zone->group = group;
	ctx->zone->total_size = ctx->total_size;
	ctx->zone->free_size = free_size;
	ctx->zone->next = g_zones;
	g_zones = ctx->zone;
}

void	init_first_block(t_zone_ctx *ctx, int is_free)
{
	ctx->block = (t_block *)((char *)ctx->zone + sizeof(t_zone));
	ctx->block->size = ctx->aligned_size;
	ctx->block->is_free = is_free;
	ctx->block->next_offset = 0;
	ctx->block->prev_offset = 0;
}

size_t	init_next_free_block(t_zone_ctx *ctx)
{
	t_block		*next;
	size_t		used_end;
	uint32_t	next_off;

	used_end = sizeof(t_zone) + sizeof(t_block) + ctx->aligned_size;
	next_off = (uint32_t)ALIGN_16(used_end);
	if ((size_t)next_off + sizeof(t_block) + 16 >= ctx->total_size)
		return (0);
	next = (t_block *)((char *)ctx->zone + next_off);
	next->size = ctx->total_size - (size_t)next_off - sizeof(t_block);
	next->is_free = 1;
	next->next_offset = 0;
	next->prev_offset = (uint32_t)((char *)ctx->block - (char *)ctx->zone);
	ctx->block->next_offset = next_off;
	return (next->size);
}

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
