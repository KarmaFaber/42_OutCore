/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_snapshot.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:27:48 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_SNAPSHOT_H
# define HEAP_SNAPSHOT_H

# include "malloc.h"
#include <errno.h>
#include <sys/mman.h>

/*
** Structures for heap snapshot representation
*/
typedef struct s_snapshot_block
{
	uintptr_t	payload;
	size_t		size;
	uint32_t	next_offset;
	uint32_t	prev_offset;
	int			is_free;
	size_t		preview_size;
	uint8_t		preview[16];
}	t_snapshot_block;

typedef struct s_snapshot_zone
{
	t_heap_group	group;
	uintptr_t		address;
	uintptr_t		next;
	size_t			total_size;
	size_t			free_size;
	size_t			first_block;
	size_t			block_count;
}	t_snapshot_zone;

typedef struct s_heap_snapshot
{
	size_t				mapping_size;
	size_t				zone_count;
	size_t				block_count;
	t_snapshot_zone		zones[MAX_ZONES];
	t_snapshot_block	blocks[];
}	t_heap_snapshot;


/*
** Function prototypes for heap snapshot operations
*/
//utils_heap/heap_snapshot_copy.c
void			snapshot_copy_heap(t_heap_snapshot *snapshot,
					size_t counts[MAX_ZONES]);
void			snapshot_sort_zones(t_heap_snapshot *snapshot);

//utils_heap/heap_snapshot_count.c
int				snapshot_count_heap(size_t *zone_count, size_t *block_count,
					size_t counts[MAX_ZONES]);
int				snapshot_count_capacity(size_t *block_capacity,
					size_t *mapping_size, size_t counts[MAX_ZONES]);

//utils_heap/heap_snapshot.c
t_heap_snapshot	*heap_snapshot_create(void);
void			heap_snapshot_destroy(t_heap_snapshot *snapshot);

//utils_show_alloc/show_alloc_mem_ex_utils.c
void			dump_payload_preview(t_snapshot_block *block);

#endif
