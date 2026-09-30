/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 22:31:13 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/11 00:51:09 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/**
 * @brief Initializes all fields and counters of the t_ping structure to zero.
 *
 * @param ping Pointer to the t_ping structure to be initialized.
 */
void	init_ping(t_ping *ping)
{
	if (!ping)
		return ;
	ping->destination = NULL;
	ping->verbose = false;
	ping->addrinfo = NULL;
	ping->socket_fd = -1;
	ping->stop_loop = false;
	ping->sequence = 0;
	ping->packets_transmitted = 0;
	ping->packets_received = 0;
	ping->rtt_min = 0.0;
	ping->rtt_max = 0.0;
	ping->rtt_sum = 0.0;
	ping->rtt_sum_sq = 0.0;
	memset(&ping->start_time, 0, sizeof(struct timeval));
}

//SOCKET
//socket(int domain, int type, int protocol)
//domain: AF_INET (IPv4). Como ya lo configuraste en hints.ai_family, podemos usar directamente ft_ping.addrinfo->ai_family.  
//type: SOCK_RAW.
//protocol: IPPROTO_ICMP (vive en <netinet/in.h>).

/**
 * @brief Creates a raw ICMP socket and sets its reception timeout to 1 second.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 * @return int Returns 0 on success, or 1 if socket creation or setup fails.
 */
int	create_socket(t_ping *ping)
{
	struct timeval	timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	ping->socket_fd = socket(ping->addrinfo->ai_family, SOCK_RAW, IPPROTO_ICMP);
	if (ping->socket_fd < 0)
	{
		perror("ft_ping: socket creation failed");
		return (1);
	}
	if (setsockopt(ping->socket_fd, SOL_SOCKET, SO_RCVTIMEO,
			&timeout, sizeof(timeout)) < 0)
	{
		perror("ft_ping: setsockopt SO_RCVTIMEO failed");
		return (1);
	}
	return (0);
}

/**
 * @brief Signal handler for SIGINT (Ctrl+C) to safely stop the execution loop.
 *
 * @param sig The signal number received (unused).
 */
void	sigint_handler(int sig)
{
	(void)sig;
	g_ping_ptr->stop_loop = true;
}

/**
 * @brief Signal handler for SIGALRM to transmit an ICMP packet every second.
 *
 * @param sig The signal number received (unused).
 */
void	sigalrm_handler(int sig)
{
	(void)sig;
	if (g_ping_ptr)
	{
		send_ping(g_ping_ptr);
	}
	alarm(1);
}
