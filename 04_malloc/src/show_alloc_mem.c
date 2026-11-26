/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:17:00 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:37:14 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include "../inc/heap_snapshot.h"

/**
 * @brief Prints a memory address in hexadecimal format.
 *
 * @param addr Memory address to print.
 */
void	put_hex(uintptr_t addr)
{
	const char	*hex;
	char		buf[17];
	int			i;

	hex = "0123456789abcdef";
	ft_putstr_fd("0x", 1);
	if (addr == 0)
	{
		ft_putstr_fd("0", 1);
		return ;
	}
	i = 15;
	buf[16] = '\0';
	while (addr > 0 && i >= 0)
	{
		buf[i] = hex[addr % 16];
		addr /= 16;
		i--;
	}
	ft_putstr_fd(&buf[i + 1], 1);
}

/**
 * @brief Prints an unsigned integer in decimal format.
 *
 * @param n Number to print.
 */
void	put_dec(size_t n)
{
	char	buf[24];
	int		i;

	if (n == 0)
	{
		ft_putstr_fd("0", 1);
		return ;
	}
	i = 23;
	buf[23] = '\0';
	while (n > 0 && i >= 0)
	{
		buf[i] = (char)('0' + (n % 10));
		n /= 10;
		i--;
	}
	ft_putstr_fd(&buf[i + 1], 1);
}

/**
 * @brief Prints allocated blocks within a memory zone.
 *
 * @param snapshot Heap snapshot containing the block metadata.
 * @param zone Zone whose allocated blocks are displayed.
 * @return Total number of allocated bytes in the zone.
 */
static size_t	print_zone(t_heap_snapshot *snapshot, t_snapshot_zone *zone)
{
	t_snapshot_block	*block;
	size_t				sum;
	size_t				i;

	sum = 0;
	i = 0;
	while (i < zone->block_count)
	{
		block = &snapshot->blocks[zone->first_block + i];
		if (!block->is_free && block->size > 0)
		{
			put_hex(block->payload);
			ft_putstr_fd(" - ", 1);
			put_hex(block->payload + block->size);
			ft_putstr_fd(" : ", 1);
			put_dec(block->size);
			ft_putstr_fd(" bytes\n", 1);
			sum += block->size;
		}
		i++;
	}
	return (sum);
}

/**
 * @brief Prints all memory zones and the total allocated bytes.
 *
 * @param snapshot Heap snapshot containing the memory information.
 */
static void	print_snapshot(t_heap_snapshot *snapshot)
{
	size_t	total;
	size_t	i;

	total = 0;
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
		total += print_zone(snapshot, &snapshot->zones[i]);
		i++;
	}
	ft_putstr_fd("Total : ", 1);
	put_dec(total);
	ft_putstr_fd(" bytes\n", 1);
}

/**
 * @brief Displays all allocated memory zones and total allocated bytes.
 *
 * @param None.
 */
void	show_alloc_mem(void)
{
	t_heap_snapshot	*snapshot;

	snapshot = heap_snapshot_create();
	if (!snapshot)
	{
		(void)write(2, "show_alloc_mem: snapshot failed\n", 32);
		return ;
	}
	print_snapshot(snapshot);
	heap_snapshot_destroy(snapshot);
}
