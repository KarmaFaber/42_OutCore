/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:06:19 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:06:22 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	start_network_scan(t_map *map)
{
	struct timeval	start;
	struct timeval	current;
	double			elapsed;
	char			*device;

	// 1. Buscar interfaz e Inicializar PCAP en modo no bloqueante
	device = find_active_device();
	if (!device || init_pcap_sniffer(map, device) != 0)
	{
		// Manejar error, liberar memoria...
		return (1);
	}

	if (gettimeofday(&start, NULL) < 0)
		return (1);


	// 2 y 3. Lanzar hilos o Bucle secuencial (aquí enviarás los paquetes)
	// AQUÍ INYECTAREMOS LOS PAQUETES (Donde aplicaremos el --ttl en la cabecera IP)
	// launch_packet_injectors(map);

	
	// 4. Esperar respuestas / timeout
	// Este es tu bucle principal de escucha
	while (1)
	{
		// Esto le dice a libpcap: "Lee el buffer de red, y por cada paquete que haya, ejecuta 'pcap_callback' silenciosamente"
		listen_packets(map);
		gettimeofday(&current, NULL);
		map->cfg.scan_time = (current.tv_sec - start.tv_sec) + 
				  ((current.tv_usec - start.tv_usec) / 1000000.0);
		if (map->cfg.scan_time >= 2.0) // Timeout de 2 segundos
			break;
	}
	

	// Al salir, cualquier puerto que siga en STATE_UNKNOWN se marca como "silencio"
	// (Aplica la lógica de tu tabla: OPEN/FILTERED o FILTERED según el tipo de scan)
	printf("\n[DEBUG]Scan took %.6f secs\n", map->cfg.scan_time);		//esto es debig, va a ir en print results
	return (0);
}

unsigned short calculate_checksum(unsigned short *ptr, int nbytes)
{
	
}