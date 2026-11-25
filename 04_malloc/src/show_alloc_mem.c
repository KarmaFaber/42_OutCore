/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:17:00 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 12:17:51 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

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
 * @brief Collects and sorts zones by their memory addresses.
 *
 * @param list Array used to store the sorted zone pointers.
 * @return Number of zones collected.
 */
int	get_sorted_zones(t_zone **list)
{
	t_zone	*curr;
	t_zone	*tmp;
	int		n;
	int		i;
	int		j;

	n = 0;
	curr = g_zones;
	while (curr && n < MAX_ZONES)
	{
		list[n++] = curr;
		curr = curr->next;
	}
	i = -1;
	while (++i < n)
	{
		j = i;
		while (++j < n)
		{
			if ((uintptr_t)list[j] < (uintptr_t)list[i])
			{
				tmp = list[i];
				list[i] = list[j];
				list[j] = tmp;
			}
		}
	}
	return (n);
}

/**
 * @brief Prints all allocated blocks within a zone.
 *
 * @param zone Zone whose allocated blocks are displayed.
 * @return Total size of allocated blocks in bytes.
 */
static size_t	print_zone(t_zone *zone)
{
	t_block		*blk;
	uintptr_t	start;
	size_t		sum;

	sum = 0;
	if (!zone || zone->total_size <= sizeof(t_zone) + sizeof(t_block))
		return (0);
	blk = (t_block *)((char *)zone + sizeof(t_zone));
	while (1)
	{
		if (!blk->is_free && blk->size > 0)
		{
			start = ALIGN_16((uintptr_t)((char *)blk + sizeof(t_block)));
			put_hex(start);
			ft_putstr_fd(" - ", 1);
			put_hex(start + blk->size);
			ft_putstr_fd(" : ", 1);
			put_dec(blk->size);
			ft_putstr_fd(" bytes\n", 1);
			sum += blk->size;
		}
		if (blk->next_offset == 0)
			break ;
		blk = (t_block *)((char *)zone + blk->next_offset);
	}
	return (sum);
}

/**
 * @brief Displays all allocated memory zones and total allocated bytes.
 *
 * @param None.
 */
void	show_alloc_mem(void)
{
	t_zone	*zones[MAX_ZONES];
	size_t	total;
	int		count;
	int		i;

	total = 0;
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
		total += print_zone(zones[i]);
	}
	ft_putstr_fd("Total : ", 1);
	put_dec(total);
	ft_putstr_fd(" bytes\n", 1);
}
