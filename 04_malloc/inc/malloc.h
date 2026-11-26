/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 11:11:21 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:43:17 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

/*
** Define
*/
#define GET_NEXT_BLOCK(zone, block) ((t_block *)((char *)(zone) + (block)->next_offset))
#define ALIGN_16(x) (((x) + 15) & ~15)				//Align to 16 bytes
#define TINY_LIMIT 1024								//TINY  - n = 1024 bytes
#define SMALL_LIMIT 16384							//SMALL - m = 16384 bytes
#define MIN_ZONE_BLOCKS 64
#define MAX_ZONES 256

/*
** includes
*/
#include "libft.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

/*
** data structs
*/
typedef enum	e_heap_group {
	TINY,
	SMALL,
	LARGE
}				t_heap_group;

/*
** A heap stores data about one mapped zone
*/
typedef struct	s_zone {
	t_heap_group	group;
	struct s_zone	*next;
	size_t			total_size;
	size_t			free_size;
}				t_zone;

/*
** A block stores data for each malloc call: "mem_block"
*/
typedef struct	s_block {
	size_t		size;
	int			is_free;
	uint32_t	next_offset;
	uint32_t	prev_offset;
}	t_block;

/*
** Global vars
*/
extern t_zone	*g_zones; // Puntero a la primera zona de nuestra memoria

/*
** Library methods
*/
//madatory:
void	free(void *ptr);
void	*malloc(size_t size);
void	ft_free(void *ptr);
void	*ft_malloc(size_t size);
int		malloc_lock(void);
int		malloc_unlock(void);

// Debug calls:
//show_alloc_mem.c
void	put_hex(uintptr_t addr);
void	put_dec(size_t n);
int		get_sorted_zones(t_zone **list);
void	show_alloc_mem(void);

//show_alloc_mem_ex.c
void	show_alloc_mem_ex(void);

/*
** Utils stuct -> context data/vars
*/
typedef struct s_zone_ctx
{
	size_t			req_size;
	size_t			aligned_size;
	size_t			total_size;
	size_t			pages_num;
	long			page_size;
	//
	t_zone			*zone;
	t_block			*block;
	uintptr_t		raw_payload;
	void			*payload;
}	t_zone_ctx;

/*
** Utils
*/
//utils_malloc/malloc_utils.c
void	init_zone_ctx(t_zone_ctx *ctx, size_t size, long page_size);

//utils_malloc/all_zones_utils.c
size_t	get_pages_num(size_t size, size_t ref, size_t page_size);
void	link_new_zone(t_zone_ctx *ctx, t_heap_group group, size_t free_size);
void	init_first_block(t_zone_ctx *ctx, int is_free);
size_t	init_next_free_block(t_zone_ctx *ctx);
void	*create_new_zone(t_zone_ctx *ctx, t_heap_group group);

//utils_malloc/small_tiny_utils.c
t_zone	*find_zone_for_alloc(t_heap_group group, size_t needed_size);
t_block	*find_free_block(t_zone *zone, size_t needed);
void	*allocate_from_zone(t_zone *zone, size_t needed);

//utils_malloc/set_large.c
void	*set_large_zone(t_zone_ctx *ctx);

//utils_malloc/set_small.c
void	*set_small_zone(t_zone_ctx *ctx);

//utils_malloc/set_tiny.c
void	*set_tiny_zone(t_zone_ctx *ctx);

#endif
