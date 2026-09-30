/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 23:36:34 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/11 00:46:10 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/**
 * @brief Accumulates data and updates the minimum, maximum, and sum RTT metrics.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 * @param time_ms Calculated Round-Trip Time of the current packet in milliseconds.
 */
void	update_rtt_stats(t_ping *ping, double time_ms)
{
	ping->packets_received++;
	ping->rtt_sum += time_ms;
	ping->rtt_sum_sq += time_ms * time_ms;
	if (ping->packets_received == 1)
	{
		ping->rtt_min = time_ms;
		ping->rtt_max = time_ms;
	}
	else
	{
		if (time_ms < ping->rtt_min)
			ping->rtt_min = time_ms;
		if (time_ms > ping->rtt_max)
			ping->rtt_max = time_ms;
	}
}

/**
 * @brief Decodes packet timestamps, prints success logs, and updates statistics.
 *
 * @param p Pointer to the t_recv_data structure containing packet information.
 * @param tv_recv Timestamp structure recorded exactly when the packet arrived.
 */
void	print_success_packet(t_recv_data *p, struct timeval *tv_recv)
{
	struct timeval	*tv_send;
	double			time_ms;
	char			*from_ip;

	tv_send = (struct timeval *)((char *)p->ip_hdr + p->ip_len + 8);
	time_ms = (tv_recv->tv_sec - tv_send->tv_sec) * 1000.0
		+ (tv_recv->tv_usec - tv_send->tv_usec) / 1000.0;
	from_ip = inet_ntoa(*(struct in_addr *)&p->ip_hdr->ip_src);
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
		p->bytes - p->ip_len, from_ip, ntohs(p->icmp->icmp_seq),
		p->ip_hdr->ip_ttl, time_ms);
	update_rtt_stats(g_ping_ptr, time_ms);
}

/**
 * @brief Parses and displays diagnostic raw network errors in verbose mode.
 *
 * @param icmp Pointer to the received ICMP header structure.
 * @param from_ip Text string containing the source IPv4 address of the sender.
 */
void	handle_verbose_error(struct icmp *icmp, char *from_ip)
{
	if (icmp->icmp_type == ICMP_ECHO)
		return ;
	if (icmp->icmp_type == ICMP_TIMXCEED)
		printf("From %s: Time to live exceeded\n", from_ip);
	else
		printf("From %s: icmp_type=%d icmp_code=%d\n",
			from_ip, icmp->icmp_type, icmp->icmp_code);
}
/**
 * @brief Receives and processes ICMP Echo Reply packets.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 * @return int 0 on success, 1 on failure.
 */
int	receive_ping(t_ping *ping)
{
	char				buf[1024];
	struct sockaddr_in	from;
	socklen_t			from_len;
	struct timeval		tv_recv;
	t_recv_data			p;

	from_len = sizeof(from);
	p.bytes = recvfrom(ping->socket_fd, buf, sizeof(buf), 0,
			(struct sockaddr *)&from, &from_len);
	if (p.bytes < 0)
		return (0);
	gettimeofday(&tv_recv, NULL);
	p.ip_hdr = (struct ip *)buf;
	p.ip_len = p.ip_hdr->ip_hl * 4;
	p.icmp = (struct icmp *)(buf + p.ip_len);
	if (p.icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if (p.icmp->icmp_id == (getpid() & 0xFFFF))
			print_success_packet(&p, &tv_recv);
	}
	else if (ping->verbose)
		handle_verbose_error(p.icmp, inet_ntoa(from.sin_addr));
	return (0);
}
