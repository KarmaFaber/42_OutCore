/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wrappers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:22 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/30 12:59:29 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Enables OS detection in the scan configuration.
 *
 * @param arg Unused argument.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success.
 */
int	handle_osdetect(char *arg, t_map *map)
{
	(void)arg;
	map->cfg.os_detect = 1;
	return (0);
}

/**
 * @brief Validates and sets the spoofed source IP address.
 *
 * @param arg IP address to use for spoofing.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 if the IP address is invalid.
 */
int	handle_spoof(char *arg, t_map *map)
{
	if (!is_valid_ip(arg))
		return (fprintf(stderr, ANSI_RED "Error: Invalid IP for spoofing\n"
				ANSI_RESET), 1);
	map->cfg.spoof_ip = arg;
	return (0);
}

/**
 * @brief Enables the use of invalid checksums in packets.
 *
 * @param arg Unused argument.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success.
 */
int	handle_badsum(char *arg, t_map *map)
{
	(void)arg;
	map->cfg.bad_sum = 1;
	return (0);
}

/**
 * @brief Enables packet tracing in the scan configuration.
 *
 * @param arg Unused argument.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success.
 */
int	handle_packettrace(char *arg, t_map *map)
{
	(void)arg;
	map->cfg.packet_trace = 1;
	return (0);
}

/**
 * @brief Validates and sets the IP time-to-live value.
 *
 * @param arg String representing the TTL value.
 * @param map Pointer to the scan data and configuration.
 * @return 0 on success, 1 if the value is invalid.
 */
int	handle_ttl(char *arg, t_map *map)
{
	int	val;

	val = parse_int_range(arg, 0, 255);
	if (val == -1)
	{
		fprintf(stderr, ANSI_RED "Error: TTL must be 0-255\n" ANSI_RESET);
		return (1);
	}
	map->cfg.ttl_value = val;
	return (0);
}
