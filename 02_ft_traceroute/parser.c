/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:40:40 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/17 21:28:17 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int	parse_args(int argc, char **argv, t_traceroute *traceroute)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			print_usage();
			exit(0);
		}
		if (argv[i][0] == '-' && argv[i][1] != '\0')
			return (fprintf(stderr, "Bad option `%c' (argc %d)\n",
					argv[i][1], argc - i), 1);
		if (!traceroute->destination)
			traceroute->destination = argv[i];
		else
			return (print_usage(), 1);
		i++;
	}
	if (!traceroute->destination)
		return (print_usage(), 1);
	return (0);
}

int	resolve_destination(t_traceroute *traceroute)
{
	struct addrinfo	hints;
	int				ret;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	ret = getaddrinfo(traceroute->destination,
			NULL, &hints, &traceroute->addrinfo);
	if (ret != 0)
	{
		fprintf(stderr, "ft_traceroute: %s: %s\n",
			traceroute->destination, gai_strerror(ret));
		return (1);
	}
	return (0);
}

int	setup_traceroute(int argc, char **argv, t_traceroute *traceroute)
{
	init_traceroute(traceroute);
	if (parse_args(argc, argv, traceroute))
		return (1);
	if (resolve_destination(traceroute))
		return (1);
	if (create_socket(traceroute))
	{
		freeaddrinfo(traceroute->addrinfo);
		return (1);
	}
	return (0);
}
