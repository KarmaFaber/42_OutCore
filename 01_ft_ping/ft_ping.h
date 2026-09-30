/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 14:40:29 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/17 10:45:07 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H

#define _POSIX_C_SOURCE				200112L
#define _GNU_SOURCE

#define DESTINATION_ADDRESS_ERROR	"ft_ping: usage error: Destination address required\n"
#define NAME_OR_SERVICE_NOT_KNOWN	"ft_ping: Name or service not known\n"

/* Standard C Library */
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* System and Signals */
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

/* Network and Sockets */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

/* ** struct definitions ** */
/**
 * @brief Main configuration and statistics structure for the ft_ping utility.
 */
typedef struct	s_ping
{
	char				*destination;
	bool				verbose;

	struct addrinfo		*addrinfo;
	int					socket_fd;
	bool				stop_loop;
	unsigned int		sequence;

	unsigned int		packets_transmitted;
	unsigned int		packets_received;

	double				rtt_min;
	double				rtt_max;
	double				rtt_sum;
	double				rtt_sum_sq;

	struct timeval		start_time;
}	t_ping;

/**
 * @brief Structure to hold received packet data.
 */
typedef struct s_recv_data
{
	int					bytes;
	int					ip_len;
	struct ip			*ip_hdr;
	struct icmp			*icmp;
}	t_recv_data;

/**
 * @brief Structure to hold statistics data.
 */
typedef struct s_stats_data
{
	struct timeval	tv_end;
	long			total_time_ms;
	double			loss;
	double			avg;
	double			avg_sq;
	double			mdev;
}	t_stats_data;

extern t_ping *g_ping_ptr;

/* ** function prototypes ** */
//---ft_ping.c
void			start_ping_loop(t_ping *ping);
//main();

//---utils.c
void			init_ping(t_ping *ping);
int				create_socket(t_ping *ping);
void			sigint_handler(int sig);
void			sigalrm_handler(int sig);

//---printers.c
void			print_usage(void);
void			print_stats(t_ping *ping);

//---parser.c
int				parse_args(int argc, char **argv, t_ping *ping);
int				resolve_destination(t_ping *ping);
int				setup_ping(int argc, char **argv, t_ping *ping);

//---send.c
unsigned short	calculate_checksum(unsigned short *addr, int count);
void			pack_icmp(char *packet, t_ping *ping);
int				send_ping(t_ping *ping);

//---receive.c
void			update_rtt_stats(t_ping *ping, double time_ms);
void			print_success_packet(t_recv_data *p, struct timeval *tv_recv);
void			handle_verbose_error(struct icmp *icmp, char *from_ip);
int				receive_ping(t_ping *ping);

#endif
