/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:08:04 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/30 11:41:44 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	execute_scan_for_target(char *target, t_map *map)
{
	struct addrinfo		hints;
	struct addrinfo		*res;
	struct sockaddr_in	*ipv4;
	char				ip_str[INET_ADDRSTRLEN];
	int					rc;
	
	//todo: hacer una fucnion aparte y sacar el codigo para extractor de IP:
	map->cfg.target_name = target;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	
	rc = getaddrinfo(target, NULL, &hints, &res);
	if (rc != 0)
	{
		fprintf(stderr, ANSI_RED "Error: %s.\n" ANSI_RESET, gai_strerror(rc));
		return (1);
	}
	ipv4 = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, INET_ADDRSTRLEN);
	map->cfg.ip = strdup(ip_str);
	if (!map->cfg.ip)
	{
		fprintf(stderr, ANSI_RED "Error: Memory allocation failed.\n" ANSI_RESET);
		freeaddrinfo(res);
		return (1);
	}
	freeaddrinfo(res);  // Liberamos la memoria de la consulta de inmediato
	
	print_scan_intro(&map->cfg);

	// 2. Aquí irá el motor de red en el siguiente bloque:
	//    - Medir tiempo de inicio con gettimeofday
	//    - Lanzar los hilos (si threads > 0) o ejecutar secuencial
	//    - Capturar respuestas con libpcap
	//    - Imprimir resultados de los puertos y "Scan took X secs"

	//DEBUG TEMPORAL
	//todo: quitar despues, solo testeo de printesr y colores. 
	print_scan_results(map, 80, STATE_OPEN);
	//print_scan_results(map, 11, STATE_UNKNOWN);
	//print_scan_results(map, 122, STATE_CLOSED);
	//print_scan_results(map, 133, STATE_FILTERED);
	//print_scan_results(map, 144, STATE_UNFILTERED);
	//print_scan_results(map, 155, STATE_OPEN_FILTERED);

	printer_debug("POST_DNS_RESOLUTION", map);
	
	
	
	free(map->cfg.ip);
	return (0);
}

int	main(int argc, char **argv)
{
	t_map	map;

	init_map(&map);
	if (parse_args(argc, argv, &map))
		return (1);
	if (map.cfg.file)
	{
		if (run_file_scans(&map))
			return (1);
	}
	else
	{
		if (execute_scan_for_target(map.cfg.ip, &map))
			return (1);
	}
	if (map.addrinfo)
		freeaddrinfo(map.addrinfo);
	//close(map.socket_fd);
	return (0);
}
