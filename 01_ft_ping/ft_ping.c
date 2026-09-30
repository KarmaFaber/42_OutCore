/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:40:48 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/11 00:46:49 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_ping	*g_ping_ptr = NULL;

/**
 * @brief Initiates the main ping loop to send and receive ICMP packets.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 */
void	start_ping_loop(t_ping *ping)
{
	char				ip_str[INET_ADDRSTRLEN];
	struct sockaddr_in	*addr;

	addr = (struct sockaddr_in *)ping->addrinfo->ai_addr;
	inet_ntop(AF_INET, &(addr->sin_addr), ip_str, INET_ADDRSTRLEN);
	printf("FT_PING %s (%s) 56(84) bytes of data.\n",
		ping->destination, ip_str);
	signal(SIGINT, sigint_handler);
	signal(SIGALRM, sigalrm_handler);
	gettimeofday(&ping->start_time, NULL);
	send_ping(ping);
	alarm(1);
	while (g_ping_ptr && !g_ping_ptr->stop_loop)
	{
		receive_ping(g_ping_ptr);
	}
}

int	main(int argc, char **argv)
{
	t_ping	ft_ping;

	if (argc < 2)
	{
		printf("%s", DESTINATION_ADDRESS_ERROR);
		return (1);
	}
	if (setup_ping(argc, argv, &ft_ping))
		return (1);
	start_ping_loop(&ft_ping);
	print_stats(&ft_ping);
	close(ft_ping.socket_fd);
	freeaddrinfo(ft_ping.addrinfo);
	return (0);
}
