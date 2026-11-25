/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 15:07:31 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 11:34:10 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/malloc.h"

size_t	get_pages_num(size_t size, size_t ref, size_t page_size)
{
	size_t	block_overhead;
	size_t	zone_overhead;
	size_t	per_block;
	size_t	required;
	size_t	pages_num;

	if (page_size == 0 || ref == 0)
		return (0);
	block_overhead = ALIGN_16(sizeof(t_block));
	zone_overhead = ALIGN_16(sizeof(t_zone));
	if (size > SIZE_MAX - block_overhead)
		return (0);
	per_block = block_overhead + size;
	if (ref > (SIZE_MAX - zone_overhead) / per_block)
		return (0);
	required = zone_overhead + ref * per_block;
	pages_num = required / page_size;
	if (required % page_size != 0)
		pages_num++;
	if (pages_num == 0 || pages_num > SIZE_MAX / page_size)
		return (0);
	return (pages_num);
}

t_zone	*find_zone_for_alloc(t_heap_group group, size_t needed_size)
{
	t_zone	*curr;

	curr = g_zones;
	while (curr != NULL)
	{
		if (curr->group == group && curr->free_size >= needed_size)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Initializes the zone initialization context.
 *
 * @param ctx Zone initialization context.
 * @param size Requested allocation size.
 */
void	init_zone_ctx(t_zone_ctx *ctx, size_t size)
{
	ft_bzero(ctx, sizeof(t_zone_ctx));
	ctx->req_size = size;
	ctx->aligned_size = ALIGN_16(size);
	ctx->page_size = sysconf(_SC_PAGESIZE);
}
