/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 23:58:26 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/11 00:16:17 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/**
 * @brief Prints the program usage instructions and available options to stdout.
 */
void	print_usage(void)
{
	printf("Usage: \n");
	printf("  sudo ./ft_ping [options] <destination>\n\n");
	printf("Options:\n");
	printf("  <destination>      dns name or ip address\n");
	printf("  -v                 verbose output\n");
	printf("  -?                 show this help\n");
	printf("  -h                 show this help\n");
}

/**
 * @brief Computes and displays final transmission statistics and RTT metrics.
 *
 * @param ping Pointer to the main t_ping configuration structure.
 */
void	print_stats(t_ping *ping)
{
	t_stats_data	s;

	gettimeofday(&s.tv_end, NULL);
	s.total_time_ms = (s.tv_end.tv_sec - ping->start_time.tv_sec) * 1000
		+ (s.tv_end.tv_usec - ping->start_time.tv_usec) / 1000;
	s.loss = 100.0;
	if (ping->packets_transmitted > 0)
	{
		s.loss = ((double)(ping->packets_transmitted - ping->packets_received)
				/ ping->packets_transmitted) * 100.0;
	}
	printf("\n--- %s ft_ping statistics ---\n", ping->destination);
	printf("%u packets transmitted, %u received, %.0f%% packet loss, ",
		ping->packets_transmitted, ping->packets_received, s.loss);
	printf("time %ldms\n", s.total_time_ms);
	if (ping->packets_received > 0)
	{
		s.avg = ping->rtt_sum / ping->packets_received;
		s.avg_sq = ping->rtt_sum_sq / ping->packets_received;
		s.mdev = sqrt(s.avg_sq - (s.avg * s.avg));
		printf("rtt min/avg/max/mdev = %.2f/%.2f/%.2f/%.2f ms\n",
			ping->rtt_min, s.avg, ping->rtt_max, s.mdev);
	}
}
