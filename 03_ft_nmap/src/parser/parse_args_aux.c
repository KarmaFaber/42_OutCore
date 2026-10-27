/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args_aux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:06:34 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 14:09:19 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Checks whether a string is a valid IPv4 address.
 *
 * @param ip IP address to validate.
 */
int is_valid_ip(const char *ip)
{
	struct sockaddr_in sa;

	return (inet_pton(AF_INET, ip, &sa.sin_addr) == 1);
}

/**
 * @brief Converts a string to a valid port number.
 *
 * @param str String representing the port number.
 * @return Port number on success, -1 on error.
 */
int is_valid_port(const char *str)
{
	char	*endptr;
	long	port;

	errno = 0;
	port = strtol(str, &endptr, 10);

	if (errno != 0 || *endptr != '\0' || port < 0 || port > 65535)
		return (-1);
	return ((int)port);
}

/**
 * @brief Returns the scan type corresponding to the given string.
 *
 * @param str String representing the scan type.
 * @return Scan type constant, or 0 if invalid.
 */
int get_scan_type(const char *str)
{
	if (!strcmp(str, "SYN"))
		return (SCAN_SYN);
	if (!strcmp(str, "NULL"))
		return (SCAN_NULL);
	if (!strcmp(str, "ACK"))
		return (SCAN_ACK);
	if (!strcmp(str, "FIN"))
		return (SCAN_FIN);
	if (!strcmp(str, "XMAS"))
		return (SCAN_XMAS);
	if (!strcmp(str, "UDP"))
		return (SCAN_UDP);
	return (0);
}

/**
 * @brief Converts a string to an integer within the given range.
 *
 * @param str String representing the integer.
 * @param min Minimum allowed value.
 * @param max Maximum allowed value.
 * @return Parsed integer on success, -1 on error.
 */
int parse_int_range(const char *str, int min, int max)
{
	char	*endptr;
	long	value;

	errno = 0;
	value = strtol(str, &endptr, 10);

	if (errno != 0 || *endptr != '\0' || value < min || value > max)
		return (-1);
	return ((int)value);
}
