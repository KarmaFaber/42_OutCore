/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:42:43 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:34:53 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

t_zone	*g_zones = NULL;

/**
 * @brief Allocates memory without acquiring the allocator lock.
 *
 * @param size Number of bytes to allocate.
 * @param page_size System page size in bytes.
 * @return Pointer to the allocated memory, or NULL on failure.
 */
static void	*malloc_unlocked(size_t size, long page_size)
{
	t_zone_ctx	ctx;

	if (size == 0 || size > SIZE_MAX - 15)
		return (NULL);
	init_zone_ctx(&ctx, size, page_size);
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

/**
 * @brief Allocates memory while protecting the allocator with a lock.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory, or NULL on failure.
 */
void	*ft_malloc(size_t size)
{
	void	*ptr;
	long	page_size;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size <= 0)
		return (NULL);
	if (malloc_lock() != 0)
		return (NULL);
	ptr = malloc_unlocked(size, page_size);
	(void)malloc_unlock();
	return (ptr);
}

/**
 * @brief Allocates memory by calling ft_malloc.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory, or NULL on failure.
 */
void	*malloc(size_t size)
{
	return (ft_malloc(size));
}
