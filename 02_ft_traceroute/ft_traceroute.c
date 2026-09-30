/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 21:26:06 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/17 21:34:39 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void	print_probe_result(t_recv_data *p, char *last_ip, int res)
{
	if (res == 0)
		printf("  *");
	else
	{
		if (strcmp(p->from_ip, last_ip) != 0)
		{
			printf("  %s (%s)", p->router_name, p->from_ip);
			strncpy(last_ip, p->from_ip, INET_ADDRSTRLEN);
		}
		printf("  %.3f ms", p->time_ms);
	}
	fflush(stdout);
}

static bool	execute_hop_probes(t_traceroute *tr, int ttl)
{
	t_recv_data	p;
	char		last_ip[INET_ADDRSTRLEN];
	int			probe;
	int			res;
	bool		reached_target;

	memset(last_ip, 0, INET_ADDRSTRLEN);
	probe = 0;
	reached_target = false;
	while (probe < 3)
	{
		if (send_traceroute(tr, ttl, probe) != 0)
			return (true);
		res = receive_traceroute(tr, &p, ttl, probe);
		print_probe_result(&p, last_ip, res);
		if (res == 1)
			reached_target = true;
		probe++;
		//usleep(100000);
	}
	return (reached_target);
}

void	start_traceroute_loop(t_traceroute *tr)
{
	char				ip_str[INET_ADDRSTRLEN];
	struct sockaddr_in	*addr;
	int					ttl;

	addr = (struct sockaddr_in *)tr->addrinfo->ai_addr;
	inet_ntop(AF_INET, &(addr->sin_addr), ip_str, INET_ADDRSTRLEN);
	printf("ft_traceroute to %s (%s), 30 hops max, 60 byte packets\n",
		tr->destination, ip_str);
	signal(SIGINT, sigint_handler);
	ttl = 1;
	while (ttl <= 30)
	{
		printf("%2d", ttl);
		if (execute_hop_probes(tr, ttl))
		{
			printf("\n");
			break ;
		}
		printf("\n");
		ttl++;
	}
}

int	main(int argc, char **argv)
{
	t_traceroute	traceroute;

	if (setup_traceroute(argc, argv, &traceroute))
		return (1);
	start_traceroute_loop(&traceroute);
	close(traceroute.socket_fd);
	freeaddrinfo(traceroute.addrinfo);
	return (0);
}
