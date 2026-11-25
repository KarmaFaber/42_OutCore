/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_small.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:03:25 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 11:35:50 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

/**
 * @brief Allocates memory from an existing small zone or creates a new one.
 *
 * @param ctx Zone initialization context.
 * @return Pointer to the allocated payload.
 */
void	*set_small_zone(t_zone_ctx *ctx)
{
	ctx->pages_num = get_pages_num(ALIGN_16(SMALL_LIMIT),
			MIN_ZONE_BLOCKS, (size_t)ctx->page_size);
	if (ctx->pages_num == 0)
		return (NULL);
	ctx->total_size = (size_t)ctx->page_size * (size_t)ctx->pages_num;
	if (g_zones != NULL)
	{
		ctx->zone = find_zone_for_alloc(SMALL, ctx->aligned_size);
		if (ctx->zone != NULL)
		{
			ctx->payload = allocate_from_zone(ctx->zone, ctx->aligned_size);
			if (ctx->payload != NULL)
			{
				return (ctx->payload);
			}
		}
	}
	return (create_new_zone(ctx, SMALL));
}
