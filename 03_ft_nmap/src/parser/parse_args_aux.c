/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args_aux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:06:34 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:06:36 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int is_valid_ip(const char *ip)
{
	struct sockaddr_in sa;

	return (inet_pton(AF_INET, ip, &sa.sin_addr) == 1);
}

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
