/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:40:40 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/11 00:46:56 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/**
 * @brief Parses command-line arguments to extract options and destination.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @param ping Pointer to the main t_ping configuration structure.
 * @return int Returns 0 on success, or 1 if the destination is missing.
 */
int	parse_args(int argc, char **argv, t_ping *ping)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == 'v' && argv[i][2] == '\0')
				ping->verbose = true;
			else
			{
				print_usage();
				return (1);
			}
		}
		else if (!ping->destination)
			ping->destination = argv[i];
		i++;
	}
	if (!ping->destination)
	{
		printf("%s", DESTINATION_ADDRESS_ERROR);
		return (1);
	}
	return (0);
}

/**
 * @brief Resolves the destination hostname or IP address into network data.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 * @return int Returns 0 on success, or 1 if network resolution fails.
 */
int	resolve_destination(t_ping *ping)
{
	struct addrinfo	hints;
	int				ret;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	ret = getaddrinfo(ping->destination, NULL, &hints, &ping->addrinfo);
	if (ret != 0)
	{
		printf("ft_ping: %s: %s\n",
			ping->destination,
			gai_strerror(ret));
		return (1);
	}
	return (0);
}

/**
 * @brief Sets up the ping configuration based on command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @param ping Pointer to the main t_ping configuration structure.
 * @return int Returns 0 on success, or 1 if setup fails.
 */
int	setup_ping(int argc, char **argv, t_ping *ping)
{
	init_ping(ping);
	g_ping_ptr = ping;
	if (parse_args(argc, argv, ping))
		return (1);
	if (resolve_destination(ping))
		return (1);
	if (create_socket(ping))
	{
		freeaddrinfo(ping->addrinfo);
		return (1);
	}
	return (0);
}
