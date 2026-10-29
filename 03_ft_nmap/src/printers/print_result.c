/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_result.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:47 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/29 17:47:15 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Maps port states to ANSI colors:
 */
const char	*g_state_colors[MAX_PORT_STATES] = {
	[ STATE_UNKNOWN ]       = ANSI_CGRAY_DARK,
	[ STATE_OPEN ]          = ANSI_GREEN,
	[ STATE_CLOSED ]        = ANSI_RED,
	[ STATE_FILTERED ]      = ANSI_ORANGE_256,
	[ STATE_UNFILTERED ]    = ANSI_YELLOW,
	[ STATE_OPEN_FILTERED ] = ANSI_CGRAY_LIGHT
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
