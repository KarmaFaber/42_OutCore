/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 20:35:46 by mzolotar          #+#    #+#             */
/*   Updated: 2026/06/19 12:22:38 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#define _POSIX_C_SOURCE				200112L
#define _GNU_SOURCE

/* Standard C Library */
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
typedef struct	s_traceroute
{
	struct addrinfo		*addrinfo;
	char				*destination;
	int					socket_fd;
	unsigned int		sequence;
	struct timeval		probe_send_times[3];
}	t_traceroute;

typedef struct s_recv_data
{
	int					bytes;
	int					ip_len;
	struct ip			*ip_hdr;
	struct icmp			*icmp;
	struct timeval		tv_recv;
	char				from_ip[INET_ADDRSTRLEN];
	struct ip			*orig_ip;
	struct icmp			*orig_icmp;
	char				router_name[NI_MAXHOST];
	double				time_ms;
	int					type;
}	t_recv_data;

/* ** function prototypes ** */
//---ft_traceroute.c
//static void	print_probe_result(t_recv_data *p, char *last_ip, int res);
//static bool	execute_hop_probes(t_traceroute *tr, int ttl);
void			start_traceroute_loop(t_traceroute *traceroute);
//main

//---utils.c
void			init_traceroute(t_traceroute *traceroute);
void			sigint_handler(int sig);
void			print_usage(void);
int				create_socket(t_traceroute *traceroute);

//---parser.c
int				parse_args(int argc, char **argv, t_traceroute *traceroute);
int				resolve_destination(t_traceroute *traceroute);
int				setup_traceroute(int argc, char **argv, t_traceroute *traceroute);

//---send.c
unsigned short	calculate_checksum(unsigned short *addr, int count);
//static void	fill_ip_header(struct ip *ip_hdr, t_traceroute *tr, int ttl);
//static void	fill_icmp_header(char *packet, struct icmp *icmp, t_traceroute *tr);
void			pack_icmp(char *packet, t_traceroute *traceroute, int ttl);
int				send_traceroute(t_traceroute *traceroute, int ttl, int probe);

//---receive.c
//static void	resolve_router_name(struct in_addr ip, char *out_name, size_t max);
//static int	parse_received_headers(t_recv_data *p);
//static int	extract_packet_data(t_traceroute *tr, t_recv_data *p);
int	receive_traceroute(t_traceroute *tr, t_recv_data *p, int ttl, int probe);

#endif
