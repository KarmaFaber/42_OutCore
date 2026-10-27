/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wrappers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:28 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 14:16:23 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Parses and validates the target IP address.
 *
 * @param arg IP address to parse.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 on error.
 */
int handle_ip(char *arg, t_map *map)
{
	return (parse_ip_arg(arg, &map->cfg));
}

/**
 * @brief Parses and validates the target file path.
 *
 * @param arg Path to the target file.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 on error.
 */
int handle_file(char *arg, t_map *map)
{
	return (parse_file_arg(arg, &map->cfg));
}

/**
 * @brief Parses and validates the port specification.
 *
 * @param arg Port numbers or ranges to parse.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 on error.
 */
int handle_ports(char *arg, t_map *map)
{
	return (parse_ports_arg(arg, &map->cfg));
}

/**
 * @brief Parses and validates the thread count.
 *
 * @param arg String representing the thread count.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 on error.
 */
int handle_speedup(char *arg, t_map *map)
{
	return (parse_speedup_arg(arg, &map->cfg));
}

/**
 * @brief Parses and validates the requested scan types.
 *
 * @param arg Comma-separated list of scan types.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 on error.
 */
int handle_scan(char *arg, t_map *map)
{
	return (parse_scan_arg(arg, &map->cfg));
}
