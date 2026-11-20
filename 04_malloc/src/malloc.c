/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:42:43 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 15:17:01 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

t_zone	*g_zones = NULL;

void *malloc(size_t size)
{
	t_zone_ctx	ctx;

	if (size == 0)
		return (NULL);
	init_zone_ctx(&ctx, size);
	if (ctx.page_size <= 0)
		return (NULL);

	if (ctx.aligned_size > SMALL_LIMIT)
	{
		return (set_large_zone(&ctx));
	}
	if (ctx.aligned_size > TINY_LIMIT)
	{
		return (set_small_zone(&ctx));
	}
	return (set_tiny_zone(&ctx));
}
