/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 14:06:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:33:42 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/heap_snapshot.h"

/**
 * @brief Prints the payload preview bytes in hexadecimal format.
 *
 * @param block Snapshot block containing the payload preview.
 */
static void	print_preview_hex(t_snapshot_block *block)
{
	const char	*hex;
	char		out[2];
	size_t		i;

	hex = "0123456789abcdef";
	i = 0;
	while (i < block->preview_size)
	{
		out[0] = hex[(block->preview[i] >> 4) & 0xf];
		out[1] = hex[block->preview[i] & 0xf];
		(void)write(1, out, 2);
		ft_putstr_fd(" ", 1);
		i++;
	}
}

/**
 * @brief Prints the payload preview bytes as printable ASCII characters.
 *
 * @param block Snapshot block containing the payload preview.
 */
static void	print_preview_ascii(t_snapshot_block *block)
{
	unsigned char	character;
	size_t			i;

	i = 0;
	while (i < block->preview_size)
	{
		character = block->preview[i];
		if (character < 32 || character > 126)
			character = '.';
		(void)write(1, &character, 1);
		i++;
	}
}

/**
 * @brief Displays a hexadecimal and ASCII preview of a snapshot block.
 *
 * @param block Snapshot block whose payload preview is displayed.
 */
void	dump_payload_preview(t_snapshot_block *block)
{
	ft_putstr_fd("      bytes[", 1);
	put_dec(block->preview_size);
	ft_putstr_fd("]: ", 1);
	print_preview_hex(block);
	ft_putstr_fd("| ", 1);
	print_preview_ascii(block);
	ft_putstr_fd("\n", 1);
}
