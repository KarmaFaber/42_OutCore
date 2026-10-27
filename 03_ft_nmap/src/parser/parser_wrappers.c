/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wrappers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:28 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:07:31 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int handle_ip(char *arg, t_map *map)
{
	return (parse_ip_arg(arg, &map->cfg));
}

int handle_file(char *arg, t_map *map)
{
	return (parse_file_arg(arg, &map->cfg));
}

int handle_ports(char *arg, t_map *map)
{
	return (parse_ports_arg(arg, &map->cfg));
}

int handle_speedup(char *arg, t_map *map)
{
	return (parse_speedup_arg(arg, &map->cfg));
}

int handle_scan(char *arg, t_map *map)
{
	return (parse_scan_arg(arg, &map->cfg));
}
