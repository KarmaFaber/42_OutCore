/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:35:17 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include "../inc/heap_snapshot.h"

static pthread_mutex_t	g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Acquires the global memory allocator mutex.
 *
 * @param None.
 * @return 0 on success, or -1 if the mutex lock fails.
 */
int	malloc_lock(void)
{
	int	error;

	error = pthread_mutex_lock(&g_malloc_mutex);
	if (error != 0)
	{
		errno = error;
		return (-1);
	}
	return (0);
}

/**
 * @brief Releases the global memory allocator mutex.
 *
 * @param None.
 * @return 0 on success, or -1 if the mutex unlock fails.
 */
int	malloc_unlock(void)
{
	int	error;

	error = pthread_mutex_unlock(&g_malloc_mutex);
	if (error != 0)
	{
		errno = error;
		return (-1);
	}
	return (0);
}
