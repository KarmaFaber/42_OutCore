/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:07:31 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 15:07:40 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

void	init_zone_ctx(t_zone_ctx *ctx, size_t size)
{
	ft_bzero(ctx, sizeof(t_zone_ctx));
	ctx->req_size = size;
	ctx->aligned_size = ALIGN_16(size);
	ctx->page_size = sysconf(_SC_PAGESIZE);
}
