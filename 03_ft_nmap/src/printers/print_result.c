/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_result.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:47 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 14:27:38 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Maps port states to ANSI colors:
 *        UNKNOWN: dark gray,
 *        OPEN: bright green,
 *        CLOSED: bright red,
 *        FILTERED: orange,
 *        UNFILTERED: yellow,
 *        OPEN_FILTERED: light gray
 */
const char	*g_state_colors[MAX_PORT_STATES] = {
	[ STATE_UNKNOWN ]       = "\001\x1B[90m\002",
	[ STATE_OPEN ]          = "\001\033[1;32m\002",
	[ STATE_CLOSED ]        = "\001\033[1;31m\002",
	[ STATE_FILTERED ]      = "\001\033[38;5;202m\002",
	[ STATE_UNFILTERED ]    = "\001\033[1;33m\002",
	[ STATE_OPEN_FILTERED ] = "\001\033[38;5;245m\002"
};

/**
 * @brief Maps port states to their display labels.
 */
const char	*g_port_status[MAX_PORT_STATES] = {
	[ STATE_UNKNOWN ]       = "Unknown",
	[ STATE_OPEN ]          = "Open",
	[ STATE_CLOSED ]        = "Closed",
	[ STATE_FILTERED ]      = "Filtered",
	[ STATE_UNFILTERED ]    = "Unfiltered",
	[ STATE_OPEN_FILTERED ] = "Open|Filtered"
};

/**
 * @brief Prints the scan duration and the result for a given port.
 *
 * @param map Pointer to the scan data and configuration.
 * @param port Port number that was scanned.
 * @param state Resulting state of the port.
 */
void	print_scan_results( t_map *map, int port, t_port_state state)
{

	if (state < 0 || state >= MAX_PORT_STATES)
			state = STATE_UNKNOWN;

	printf(ANSI_MAGENTA "\n ╌╌── ⋆⋆ ╌╌──  Scan Results: ╌╌── ⋆⋆ ╌╌── \n" ANSI_RESET);
	printf("Scan took %.6f secs\n", map->cfg.scan_time);

	printf("Port %d: %s%-15s%s\n", 
		   port, 
		   g_state_colors[state], 
		   g_port_status[state], 
		   ANSI_RESET);
}
