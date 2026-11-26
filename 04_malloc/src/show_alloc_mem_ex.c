/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:14:27 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:36:12 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include "../inc/heap_snapshot.h"

/**
 * @brief Prints block metadata, including its address, size and status.
 *
 * @param block Snapshot block whose information is displayed.
 * @param idx Block index within the zone.
 */
static void	print_block_line(t_snapshot_block *block, size_t idx)
{
	ft_putstr_fd("    [blk ", 1);
	put_dec(idx);
	ft_putstr_fd("] ", 1);
	put_hex(block->payload);
	ft_putstr_fd(" - ", 1);
	put_hex(block->payload + block->size);
	ft_putstr_fd(" | size=", 1);
	put_dec(block->size);
	if (block->is_free)
		ft_putstr_fd(" | FREE", 1);
	else
		ft_putstr_fd(" | USED", 1);
	ft_putstr_fd(" | next_off=", 1);
	put_dec((size_t)block->next_offset);
	ft_putstr_fd(" | prev_off=", 1);
	put_dec((size_t)block->prev_offset);
	ft_putstr_fd("\n", 1);
}

/**
 * @brief Prints zone metadata and details of all its blocks.
 *
 * @param snapshot Heap snapshot containing the zone metadata.
 * @param zone Zone whose memory layout is displayed.
 */
static void	print_zone_ex(t_heap_snapshot *snapshot, t_snapshot_zone *zone)
{
	t_snapshot_block	*block;
	size_t				i;

	ft_putstr_fd("  total=", 1);
	put_dec(zone->total_size);
	ft_putstr_fd(" | free_size=", 1);
	put_dec(zone->free_size);
	ft_putstr_fd(" | next=", 1);
	put_hex(zone->next);
	ft_putstr_fd("\n", 1);
	i = 0;
	while (i < zone->block_count)
	{
		block = &snapshot->blocks[zone->first_block + i];
		print_block_line(block, i);
		if (!block->is_free && block->size > 0)
			dump_payload_preview(block);
		i++;
	}
}

/**
 * @brief Prints an extended memory dump of all snapshot zones.
 *
 * @param snapshot Heap snapshot containing the memory information.
 */
static void	print_snapshot_ex(t_heap_snapshot *snapshot)
{
	size_t	i;

	ft_putstr_fd("=== EX DUMP ===\n", 1);
	i = 0;
	while (i < snapshot->zone_count)
	{
		if (snapshot->zones[i].group == TINY)
			ft_putstr_fd("TINY : ", 1);
		else if (snapshot->zones[i].group == SMALL)
			ft_putstr_fd("SMALL : ", 1);
		else
			ft_putstr_fd("LARGE : ", 1);
		put_hex(snapshot->zones[i].address);
		ft_putstr_fd("\n", 1);
		print_zone_ex(snapshot, &snapshot->zones[i]);
		i++;
	}
	ft_putstr_fd("===============\n", 1);
}

/**
 * @brief Creates and displays an extended dump of allocated memory.
 *
 * @param None.
 */
void	show_alloc_mem_ex(void)
{
	t_heap_snapshot	*snapshot;

	snapshot = heap_snapshot_create();
	if (!snapshot)
	{
		(void)write(2, "show_alloc_mem_ex: snapshot failed\n", 35);
		return ;
	}
	print_snapshot_ex(snapshot);
	heap_snapshot_destroy(snapshot);
}
