/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 11:11:21 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/14 00:33:39 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

/*
** Define
*/
//
#define GET_NEXT_BLOCK(zone, block) ((t_block *)((char *)(zone) + (block)->next_offset))

//Align to 16 bytes
#define ALIGN_16(x) (((x) + 15) & ~15)

/*
** includes
*/
#include <stdio.h>
#include <sys/mman.h>	//mmap + munmap
#include <unistd.h>		//sysconf()
#include <pthread.h>	//theards
#include <stdint.h>		// uint32_t




/*
** For a pagesize of 4096 bytes
** for size of header: sizeof(t_block)
**
** TINY  - n = 1024 bytes - heap 1KB -> N = 26 páginas (approx 106 KB)
** SMALL - m = 16384 bytes - heap 16 KB -> M = 32 páginas (approx 131 KB)
** LARGE - block > 16384 bytes
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
	// Aquí colgarían los bloques internos con offsets
}				t_zone;


/*
** A block stores data for each malloc call
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
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

// Debug calls:
void show_alloc_mem();

//bonus:
void show_alloc_mem_ex();


/*
** Internal methods
*/




/*
** Utils methods
*/



#endif