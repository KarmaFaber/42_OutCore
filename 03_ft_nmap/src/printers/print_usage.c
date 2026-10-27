/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_usage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:55 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:07:58 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	print_usage(void)
{
	printf("Usage:\n");
	printf("./ft_nmap [--help] [--ports [NUMBER/RANGED]] --ip IP_ADDRESS [--speedup [NUMBER]] [--scan [TYPE]]\n");
	printf("or:\n");
	printf("./ft_nmap [--help] [--ports [NUMBER/RANGED]] --file FILE [--speedup [NUMBER]] [--scan [TYPE]]\n\n");
	printf("OPTIONS:\n");
	printf("  --help                     Print this help screen\n");
	printf("  --ports [NUMBER/RANGED]    Ports to scan (e.g., 1-10 or 1,2,3 or 1,5-15)\n");
	printf("  --ip [IP_ADDRESS]          IP addresses to scan in dot format\n");
	printf("  --file [FILE]              File name containing IP addresses to scan\n");
	printf("  --speedup [NUMBER]         [250 max] Number of parallel threads to use\n");
	printf("  --scan [TYPE]              SYN/NULL/FIN/XMAS/ACK/UDP\n");
	printf("\nBONUS OPTIONS:\n");
	printf("  --os-detect                Enable OS detection\n");
	printf("  --spoof-ip [IP_ADDRESS]    Spoof source IP address\n");
	printf("  --badsum                   Send packets with bad checksums\n");
	printf("  --packet-trace             Enable packet tracing\n");
	printf("  --ttl [VALUE]              Set TTL value for packets (0-255)\n");
}
