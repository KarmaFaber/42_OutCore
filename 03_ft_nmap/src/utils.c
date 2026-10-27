/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:08:10 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 14:18:55 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Initializes the scan map with default values.
 *
 * @param map Pointer to the map structure.
 */
void	init_map(t_map *map)
{
	if (!map)
		return ;
	memset(map, 0, sizeof(t_map));
	map->socket_fd = -1;

	map->cfg.ttl_value = -1;
	map->pcap_handle = NULL;
}

