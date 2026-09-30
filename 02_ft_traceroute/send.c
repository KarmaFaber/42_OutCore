/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 21:29:10 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/17 21:29:34 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

unsigned short	calculate_checksum(unsigned short *addr, int count)
{
	unsigned long	sum;

	sum = 0;
	while (count > 1)
	{
		sum += *addr++;
		count -= 2;
	}
	if (count > 0)
		sum += *(unsigned char *)addr;
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return ((unsigned short) ~sum);
}

static void	fill_ip_header(struct ip *ip_hdr, t_traceroute *tr, int ttl)
{
	struct sockaddr_in	*dest_addr;

	ip_hdr->ip_v = 4;
	ip_hdr->ip_hl = sizeof(struct ip) >> 2;
	ip_hdr->ip_tos = 0;
	ip_hdr->ip_len = htons(84);
	ip_hdr->ip_id = htons(getpid() & 0xFFFF);
	ip_hdr->ip_off = 0;
	ip_hdr->ip_ttl = ttl;
	ip_hdr->ip_p = IPPROTO_ICMP;
	ip_hdr->ip_sum = 0;
	dest_addr = (struct sockaddr_in *)tr->addrinfo->ai_addr;
	ip_hdr->ip_dst = dest_addr->sin_addr;
	ip_hdr->ip_src.s_addr = INADDR_ANY;
}

static void	fill_icmp_header(char *packet, struct icmp *icmp,
	t_traceroute *tr, int ttl)
{
	struct timeval	*tv_send;

	(void)tr;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = getpid() & 0xFFFF;
	icmp->icmp_seq = htons(ttl);
	icmp->icmp_cksum = 0;
	tv_send = (struct timeval *)(packet + sizeof(struct ip) + 8);
	gettimeofday(tv_send, NULL);
	memset(packet + sizeof(struct ip) + 8 + sizeof(struct timeval), 'A', 40);
}

void	pack_icmp(char *packet, t_traceroute *traceroute, int ttl)
{
	struct ip	*ip_hdr;
	struct icmp	*icmp_hdr;

	ip_hdr = (struct ip *)packet;
	icmp_hdr = (struct icmp *)(packet + sizeof(struct ip));
	fill_ip_header(ip_hdr, traceroute, ttl);
	fill_icmp_header(packet, icmp_hdr, traceroute, ttl);
	icmp_hdr->icmp_cksum = calculate_checksum((unsigned short *)icmp_hdr, 64);
	ip_hdr->ip_sum = calculate_checksum((unsigned short *)ip_hdr,
			sizeof(struct ip));
}

int	send_traceroute(t_traceroute *traceroute, int ttl, int probe)
{
	char	packet[1024];
	int		bytes_sent;

	memset(packet, 0, sizeof(packet));
	pack_icmp(packet, traceroute, ttl);
	gettimeofday(&traceroute->probe_send_times[probe], NULL);
	bytes_sent = sendto(traceroute->socket_fd, packet, 84, 0,
			traceroute->addrinfo->ai_addr, traceroute->addrinfo->ai_addrlen);
	if (bytes_sent < 0)
	{
		perror("ft_traceroute: sendto failed");
		return (1);
	}
	return (0);
}
