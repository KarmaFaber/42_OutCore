/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 22:31:13 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/17 21:30:59 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	init_traceroute(t_traceroute *traceroute)
{
	if (!traceroute)
		return ;
	memset(traceroute, 0, sizeof(t_traceroute));
	traceroute->socket_fd = -1;
}

void	sigint_handler(int sig)
{
	(void)sig;
	exit(0);
}

void	print_usage(void)
{
	printf("Usage:\n");
	printf("  ft_traceroute <host>\n\n");
	printf("Options:\n");
	printf("  --help             Read this help and exit\n");
	printf("Arguments:\n");
	printf("  <host>             The host to"
		" traceroute to (IP address or hostname)\n");
}

int	create_socket(t_traceroute *traceroute)
{
	struct timeval	timeout;
	int				one;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	one = 1;
	traceroute->socket_fd = socket(traceroute->addrinfo->ai_family,
			SOCK_RAW, IPPROTO_ICMP);
	if (traceroute->socket_fd < 0)
	{
		perror("ft_traceroute socket creation failed");
		return (1);
	}
	if (setsockopt(traceroute->socket_fd,
			SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		perror("ft_traceroute: setsockopt SO_RCVTIMEO failed");
		return (1);
	}
	if (setsockopt(traceroute->socket_fd,
			IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)
		return (perror("ft_traceroute: setsockopt IP_HDRINCL failed"), 1);
	return (0);
}
