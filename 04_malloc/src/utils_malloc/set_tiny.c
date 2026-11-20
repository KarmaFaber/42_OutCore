/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tiny.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 12:25:34 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 15:03:19 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

void	*set_tiny_zone(t_zone_ctx *ctx)
{
	ctx->pages_num = calculate_pages_num(ALIGN_16(TINY_LIMIT),
			MIN_ZONE_BLOCKS, ctx->page_size);
	ctx->total_size = (size_t)ctx->page_size * (size_t)ctx->pages_num;
	if (g_zones != NULL)
	{
		ctx->zone = find_zone_for_alloc(TINY, ctx->aligned_size);
		if (ctx->zone != NULL)
		{
			ctx->block = find_free_block(ctx->zone, ctx->aligned_size);
			if (ctx->block != NULL)
			{
				ctx->block->is_free = 0;
				if (ctx->zone->free_size >= ctx->aligned_size)
					ctx->zone->free_size -= ctx->aligned_size;
				ctx->raw_payload = (uintptr_t)((char *)ctx->block
						+ sizeof(t_block));
				ctx->payload = (void *)ALIGN_16(ctx->raw_payload);
				return (ctx->payload);
			}
		}
	}
	return (create_new_zone(ctx, TINY));
}
