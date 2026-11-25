/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:14:27 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 12:16:29 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

/**
 * @brief Prints a byte as a two-digit hexadecimal value.
 *
 * @param b Byte to print.
 */
static void	put_hex_byte(unsigned char b)
{
	const char	*hex;
	char		out[2];

	hex = "0123456789abcdef";
	out[0] = hex[(b >> 4) & 0xf];
	out[1] = hex[b & 0xf];
	(void)write(1, out, 2);
}

/**
 * @brief Prints a hexadecimal and ASCII preview of a block's payload.
 *
 * @param blk Block whose payload is displayed.
 */
static void	dump_payload_preview(t_block *blk)
{
	unsigned char	*p;
	size_t			limit;
	size_t			i;

	p = (unsigned char *)ALIGN_16((uintptr_t)((char *)blk + sizeof(t_block)));
	limit = 16;
	if (blk->size < 16)
		limit = blk->size;
	ft_putstr_fd("      bytes[", 1);
	put_dec(limit);
	ft_putstr_fd("]: ", 1);
	i = 0;
	while (i < limit)
	{
		put_hex_byte(p[i]);
		ft_putstr_fd(" ", 1);
		i++;
	}
	ft_putstr_fd("| ", 1);
	i = 0;
	while (i < limit)
	{
		if (p[i] >= 32 && p[i] <= 126)
			(void)write(1, &p[i], 1);
		else
			ft_putstr_fd(".", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
}

/**
 * @brief Prints block metadata, including its address, size and status.
 *
 * @param blk Block whose information is displayed.
 * @param start Aligned starting address of the payload.
 * @param idx Block index within the zone.
 */
static void	print_block_line(t_block *blk, uintptr_t start, int idx)
{
	ft_putstr_fd("    [blk ", 1);
	put_dec((size_t)idx);
	ft_putstr_fd("] ", 1);
	put_hex(start);
	ft_putstr_fd(" - ", 1);
	put_hex(start + blk->size);
	ft_putstr_fd(" | size=", 1);
	put_dec(blk->size);
	if (blk->is_free)
		ft_putstr_fd(" | FREE", 1);
	else
		ft_putstr_fd(" | USED", 1);
	ft_putstr_fd(" | next_off=", 1);
	put_dec((size_t)blk->next_offset);
	ft_putstr_fd(" | prev_off=", 1);
	put_dec((size_t)blk->prev_offset);
	ft_putstr_fd("\n", 1);
}

/**
 * @brief Prints zone metadata and details of all its blocks.
 *
 * @param zone Zone whose memory layout is displayed.
 */
static void	print_zone_ex(t_zone *zone)
{
	t_block		*blk;
	uintptr_t	start;
	int			idx;

	ft_putstr_fd("  total=", 1);
	put_dec(zone->total_size);
	ft_putstr_fd(" | free_size=", 1);
	put_dec(zone->free_size);
	ft_putstr_fd(" | next=", 1);
	put_hex((uintptr_t)zone->next);
	ft_putstr_fd("\n", 1);
	if (zone->total_size <= sizeof(t_zone) + sizeof(t_block))
		return ;
	blk = (t_block *)((char *)zone + sizeof(t_zone));
	idx = 0;
	while (1)
	{
		start = ALIGN_16((uintptr_t)((char *)blk + sizeof(t_block)));
		print_block_line(blk, start, idx);
		if (!blk->is_free && blk->size > 0)
			dump_payload_preview(blk);
		if (blk->next_offset == 0)
			break ;
		blk = (t_block *)((char *)zone + blk->next_offset);
		idx++;
	}
}

/**
 * @brief Displays a sorted extended dump of all allocated memory zones.
 *
 * @param None.
 */
void	show_alloc_mem_ex(void)
{
	t_zone	*zones[MAX_ZONES];
	int		count;
	int		i;

	ft_putstr_fd("=== EX DUMP ===\n", 1);
	count = get_sorted_zones(zones);
	i = -1;
	while (++i < count)
	{
		if (zones[i]->group == TINY)
			ft_putstr_fd("TINY : ", 1);
		else if (zones[i]->group == SMALL)
			ft_putstr_fd("SMALL : ", 1);
		else
			ft_putstr_fd("LARGE : ", 1);
		put_hex((uintptr_t)zones[i]);
		ft_putstr_fd("\n", 1);
		print_zone_ex(zones[i]);
	}
	ft_putstr_fd("===============\n", 1);
}
