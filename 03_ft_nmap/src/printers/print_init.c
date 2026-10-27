/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:41 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:07:43 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	print_scan_types(int scan_types)
{
	printf("Scans to be performed : ");
	if (scan_types & SCAN_SYN)
		printf(ANSI_BLUE "SYN " ANSI_RESET);
	if (scan_types & SCAN_NULL)
		printf(ANSI_BLUE "NULL " ANSI_RESET);
	if (scan_types & SCAN_FIN)
		printf(ANSI_BLUE "FIN " ANSI_RESET);
	if (scan_types & SCAN_XMAS)
		printf(ANSI_BLUE "XMAS " ANSI_RESET);
	if (scan_types & SCAN_ACK)
		printf(ANSI_BLUE "ACK " ANSI_RESET);
	if (scan_types & SCAN_UDP)
		printf(ANSI_BLUE "UDP " ANSI_RESET);
	printf("\n");
}

void	print_bonus_intro(t_nmap_cfg *cfg)
{
	printf("Bonus : \n");
	
	printf("    OS DETECT : ");
	if (cfg->os_detect)
		printf(ANSI_GREEN "ON" ANSI_RESET "\n");
	else
		printf(ANSI_RED "OFF" ANSI_RESET "\n");

	printf("    SPOOF IP  : ");
	if (cfg->spoof_ip)
		printf(ANSI_BLUE "%s" ANSI_RESET "\n", cfg->spoof_ip);
	else
		printf(ANSI_RED "(null)" ANSI_RESET "\n"); // O gris/rojo si prefieres advertir que no hay spoof
	
	printf("    BAD SUM   : ");
	if (cfg->bad_sum)
		printf(ANSI_GREEN "ON" ANSI_RESET "\n");
	else
		printf(ANSI_RED "OFF" ANSI_RESET "\n");
	
	printf("    PACKET TRACE   : ");
	if (cfg->packet_trace)
		printf(ANSI_GREEN "ON" ANSI_RESET "\n");
	else
		printf(ANSI_RED "OFF" ANSI_RESET "\n");

	printf("    TTL VALUE   : ");
	if (cfg->ttl_value >= 0 && cfg->ttl_value <= 255)
		printf(ANSI_BLUE "%d" ANSI_RESET "\n", cfg->ttl_value);
	else
		printf(ANSI_RED "(default)" ANSI_RESET "\n");
}

void	print_scan_intro(t_nmap_cfg *cfg)
{
	printf(ANSI_MAGENTA "\n ╌╌── ⋆⋆ ╌╌──  Scan Configurations: ╌╌── ⋆⋆ ╌╌── \n" ANSI_RESET);
	if (cfg->file)
		printf("IPv4 addresses or hostname from a file:\n");
	else
		printf("IPv4 address:\n");

	if (cfg->target_name && cfg->ip && strcmp(cfg->target_name, cfg->ip) != 0)
		printf("    - Target Hostname   : " ANSI_BLUE "%s" ANSI_RESET "\n", cfg->target_name);
	
	if (cfg->ip)
		printf("    - Target Ip-Address : " ANSI_BLUE "%s" ANSI_RESET "\n", cfg->ip);
	else
		printf("    - Target Ip-Address : " ANSI_BLUE "(null)" ANSI_RESET "\n");
	
	if (cfg->file)
		printf("    - Target file       : " ANSI_BLUE "%s" ANSI_RESET "\n", cfg->file);
	
	printf("No of threads : " ANSI_BLUE "%d" ANSI_RESET "\n", cfg->threads);
	printf("No of Ports to scan : " ANSI_BLUE "%d" ANSI_RESET "\n", cfg->num_ports);
	
	print_scan_types(cfg->scan_types);
	print_bonus_intro(cfg);

	printf(ANSI_MAGENTA "Scanning..\n.......\n" ANSI_RESET);
}
