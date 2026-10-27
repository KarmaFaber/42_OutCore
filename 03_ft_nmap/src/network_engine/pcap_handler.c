/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pcap_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:06:11 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:06:14 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/* Procesa cada paquete capturado */
void	pcap_callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	t_map			*map;
	struct iphdr	*ip;
	int				ip_hdr_len;

	map = (t_map *)args;
	(void)header;

	// Saltamos los 14 bytes de la cabecera Ethernet
	ip = (struct iphdr *)(packet + 14);
	
	// Calculamos el tamaño real de la cabecera IP (IHL cuenta en palabras de 32 bits)
	ip_hdr_len = ip->ihl * 4;

	// Validamos si es TCP o UDP
	if (ip->protocol == IPPROTO_TCP)
	{
		struct tcphdr *tcp = (struct tcphdr *)(packet + 14 + ip_hdr_len);
		int port = ntohs(tcp->source);
		
		// Aquí analizas los flags (tcp->syn, tcp->rst, etc.) y actualizas map->results
	}
	else if (ip->protocol == IPPROTO_UDP)
	{
		struct udphdr *udp = (struct udphdr *)(packet + 14 + ip_hdr_len);
		int port = ntohs(udp->source);
		
		// Aquí analizas la respuesta UDP o ICMP correspondiente
	}
}

/* Configura el dispositivo y aplica el filtro BPF */
int	init_pcap_sniffer(t_map *map, char *device)
{
	char				errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program	fp;
	char				filter_exp[64];

	map->pcap_handle = pcap_open_live(device, BUFSIZ, 1, 100, errbuf);
	if (!map->pcap_handle)
		return (1);
	if (pcap_setnonblock(map->pcap_handle, 1, errbuf) < 0)
		return (1);
	snprintf(filter_exp, sizeof(filter_exp), "src host %s and (tcp or udp)", map->cfg.ip);
	if (pcap_compile(map->pcap_handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
		return (1);
	if (pcap_setfilter(map->pcap_handle, &fp) == -1)
		return (1);
	pcap_freecode(&fp);
	return (0);
}

/* Captura paquetes disponibles en el buffer */
void	listen_packets(t_map *map)
{
	if (!map->pcap_handle)
		return ;
	// pcap_dispatch procesa los paquetes listos sin bloquear el flujo
	pcap_dispatch(map->pcap_handle, -1, pcap_callback, (u_char *)map);
}

char	*find_active_device(void)
{
	pcap_if_t	*alldevs;
	pcap_if_t	*d;
	char		*dev_name;
	char		errbuf[PCAP_ERRBUF_SIZE];

	dev_name = NULL;
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
		return (NULL);
	d = alldevs;
	while (d)
	{
		// Buscamos la primera interfaz que no sea loopback y tenga IPs asignadas
		if (!(d->flags & PCAP_IF_LOOPBACK) && d->addresses)
		{
			dev_name = strdup(d->name);
			break ;
		}
		d = d->next;
	}
	pcap_freealldevs(alldevs);
	return (dev_name);
}

