/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 21:28:33 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/17 21:28:57 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void	resolve_router_name(struct in_addr ip, char *out_name, size_t max)
{
	struct sockaddr_in	sa;
	int					ret;

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr = ip;
	ret = getnameinfo((struct sockaddr *)&sa, sizeof(sa), out_name, max,
			NULL, 0, NI_NAMEREQD);
	if (ret != 0)
		strncpy(out_name, inet_ntoa(ip), max);
}

static int	parse_received_headers(t_recv_data *p, int ttl)
{
	if (p->icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if (p->icmp->icmp_id != (getpid() & 0xFFFF)
			|| ntohs(p->icmp->icmp_seq) != ttl)
			return (0);
		return (1);
	}
	if (p->icmp->icmp_type == ICMP_TIMXCEED
		&& p->icmp->icmp_code == ICMP_TIMXCEED_INTRANS)
	{
		p->orig_ip = (struct ip *)((char *)p->icmp + 8);
		p->orig_icmp = (struct icmp *)((char *)p->orig_ip
				+ (p->orig_ip->ip_hl * 4));
		if (p->orig_icmp->icmp_id != (getpid() & 0xFFFF))
			return (0);
		if (ntohs(p->orig_icmp->icmp_seq) != ttl)
			return (0);
		return (2);
	}
	return (0);
}

static int	extract_packet_data(t_traceroute *tr, t_recv_data *p, int ttl)
{
	struct sockaddr_in	from;
	socklen_t			from_len;
	char				buf[1024];

	from_len = sizeof(from);
	memset(buf, 0, sizeof(buf));
	p->bytes = recvfrom(tr->socket_fd, buf, sizeof(buf), 0,
			(struct sockaddr *)&from, &from_len);
	if (p->bytes < 0)
		return (0);
	gettimeofday(&p->tv_recv, NULL);
	strncpy(p->from_ip, inet_ntoa(from.sin_addr), INET_ADDRSTRLEN);
	p->ip_hdr = (struct ip *)buf;
	p->ip_len = p->ip_hdr->ip_hl * 4;
	if (p->bytes < p->ip_len + 8)
		return (0);
	p->icmp = (struct icmp *)(buf + p->ip_len);
	p->type = parse_received_headers(p, ttl);
	return (p->type);
}

int	receive_traceroute(t_traceroute *tr, t_recv_data *p, int ttl, int probe)
{
	int	res;

	res = extract_packet_data(tr, p, ttl);
	if (res == 0)
		return (0);
	p->time_ms = (p->tv_recv.tv_sec - tr->probe_send_times[probe].tv_sec)
		* 1000.0 + (p->tv_recv.tv_usec - tr->probe_send_times[probe].tv_usec)
		/ 1000.0;
	if (p->time_ms < 0.0)
		p->time_ms = 0.0;
	resolve_router_name(p->ip_hdr->ip_src, p->router_name,
		sizeof(p->router_name));
	return (res);
}
