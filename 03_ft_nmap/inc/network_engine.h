/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_engine.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:05:42 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:05:44 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_ENGINE_H
# define NETWORK_ENGINE_H


//1. network_engine/pcap_handler.c
void		pcap_callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
int			init_pcap_sniffer(t_map *map, char *device);
void		listen_packets(t_map *map);
char		*find_active_device(void);

//2.
//network_engine/scan.c
int				start_network_scan(t_map *map);
unsigned short	calculate_checksum(unsigned short *ptr, int nbytes);


//network_engine/thread_pool.c


//network_engine/network.c


//3.
//network_engine/packet_builder.c





#endif