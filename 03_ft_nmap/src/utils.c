/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:08:10 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:08:12 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	init_map(t_map *map)
{
	if (!map)
		return ;
	memset(map, 0, sizeof(t_map));
	map->socket_fd = -1;

	map->cfg.ttl_value = -1;
	map->pcap_handle = NULL;
}

