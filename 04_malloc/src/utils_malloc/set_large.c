/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_large.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:03:07 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 15:03:09 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

void *set_large_zone(t_zone_ctx *ctx)
{
	ctx->pages_num = calculate_pages_num(ctx->aligned_size, 1, ctx->page_size);
	ctx->total_size = (size_t)ctx->page_size * (size_t)ctx->pages_num;
	return (create_new_zone(ctx, LARGE));
}
