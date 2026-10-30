/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:05:32 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/30 11:41:21 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NMAP_H
#define FT_NMAP_H

#define _POSIX_C_SOURCE				200112L
#define _GNU_SOURCE

//#➵⤐──╌╌➣⋆➣── Includes ─────────── ➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌╌╌➔#
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <pcap.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <pthread.h>

#include <sys/time.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

//#➵⤐──╌╌➣⋆➣── Includes ─────────── ➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌╌╌➔#
#include "parser.h"
#include "datatypes.h"
#include "network_engine.h"
#include "printer.h"


//--------------------------------------------------------------
//utils.c
void		init_map(t_map *map);

//ft_nmap.c
int			execute_scan_for_target(char *target_ip, t_map *map);
//int	main(int argc, char **argv);

//print_debug.c
void	printer_debug(const char *tag, const t_map *map);

#endif
