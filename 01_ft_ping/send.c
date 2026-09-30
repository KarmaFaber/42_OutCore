/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 21:16:42 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/11 00:46:02 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/**
 * @brief Computes the standard Internet Checksum (RFC 1071) over binary data.
 *
 * @param addr Pointer to the data buffer to be processed.
 * @param count Size of the data buffer in bytes.
 * @return unsigned short The calculated 16-bit one's complement checksum value.
 */
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

/**
 * @brief Structures an ICMP Echo Request payload and embeds a transmission stamp.
 *
 * @param packet Character buffer where the raw network frame will be written.
 * @param ping Pointer to the main t_ping configuration structure.
 */
void	pack_icmp(char *packet, t_ping *ping)
{
	struct icmp		*icmp_hdr;
	struct timeval	*tv_send;

	icmp_hdr = (struct icmp *)packet;
	icmp_hdr->icmp_type = ICMP_ECHO;
	icmp_hdr->icmp_code = 0;
	icmp_hdr->icmp_id = getpid() & 0xFFFF;
	icmp_hdr->icmp_seq = htons(ping->sequence++);
	icmp_hdr->icmp_cksum = 0;
	tv_send = (struct timeval *)(packet + 8);
	gettimeofday(tv_send, NULL);
	memset(packet + 8 + sizeof(struct timeval), 'A',
		56 - sizeof(struct timeval));
	icmp_hdr->icmp_cksum = calculate_checksum((unsigned short *)packet, 64);
}
/**
 * @brief Sends an ICMP Echo Request packet.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 * @return int 0 on success, 1 on failure.
 */
int	send_ping(t_ping *ping)
{
	char	packet[64];
	int		bytes_sent;

	memset(packet, 0, sizeof(packet));
	pack_icmp(packet, ping);
	bytes_sent = sendto(ping->socket_fd, packet, sizeof(packet), 0,
			ping->addrinfo->ai_addr, ping->addrinfo->ai_addrlen);
	if (bytes_sent < 0)
	{
		perror("ft_ping: sendto failed");
		return (1);
	}
	ping->packets_transmitted++;
	return (0);
}
