/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_large.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:03:07 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 11:34:45 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

/**
 * @brief Configures and creates a large zone for the requested allocation.
 *
 * @param ctx Zone initialization context.
 * @return Pointer to the allocated payload.
 */
void	*set_large_zone(t_zone_ctx *ctx)
{
	ctx->pages_num = get_pages_num(ctx->aligned_size, 1,
			(size_t)ctx->page_size);
	if (ctx->pages_num == 0)
		return (NULL);
	ctx->total_size = (size_t)ctx->page_size * (size_t)ctx->pages_num;
	return (create_new_zone(ctx, LARGE));
}
