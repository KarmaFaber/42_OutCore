/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:33:18 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/30 11:39:54 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

extern const char *g_port_status[MAX_PORT_STATES];
extern const char *g_state_colors[MAX_PORT_STATES];

/* --- Formatting helpers with your colors --- */
static void	dbg_str(const char *field, const char *val)
{
	printf("  %-18s: ", field);
	if (!val || val[0] == '\0')
		printf(ANSI_RED "[VOID / NULL]\n" ANSI_RESET);
	else
		printf(ANSI_BLUE "\"%s\"\n" ANSI_RESET, val);
}

static void	dbg_int(const char *field, int val, int is_void)
{
	printf("  %-18s: ", field);
	if (is_void)
		printf(ANSI_RED "[VOID / NOT SET] (%d)\n" ANSI_RESET, val);
	else
		printf(ANSI_BLUE "%d\n" ANSI_RESET, val);
}

static void	dbg_ptr(const char *field, const void *ptr)
{
	printf("  %-18s: ", field);
	if (!ptr)
		printf(ANSI_RED "[VOID / NULL]\n" ANSI_RESET);
	else
		printf(ANSI_BLUE "%p\n" ANSI_RESET, ptr);
}

/* Helper for inspecting byte-by-byte endianness in memory */
static void	dbg_inspect_port_memory(const int *port_ptr)
{
	unsigned char	*raw;
	uint16_t		val_host;

	if (!port_ptr)
		return ;
	raw = (unsigned char *)port_ptr;
	val_host = (uint16_t)(*port_ptr);

	printf("    " ANSI_CGRAY_LIGHT "Memory dump at %p:" ANSI_RESET " [%02X %02X] -> ",
		(void *)port_ptr, raw[0], raw[1]);
	if (raw[0] == (uint8_t)(val_host & 0xFF) && raw[1] == (uint8_t)((val_host >> 8) & 0xFF))
	{
		printf(ANSI_GREEN "[Host / Little-Endian] " ANSI_RESET "(port = %u)\n", val_host);
	}
	else if (raw[0] == (uint8_t)((val_host >> 8) & 0xFF) && raw[1] == (uint8_t)(val_host & 0xFF))
	{
		printf(ANSI_ORANGE_256 "[Network / Big-Endian] " ANSI_RESET "(net = %u | host = %u)\n",
			val_host, ntohs(val_host));
	}
	else
	{
		printf("Raw: %u | Net (ntohs): %u\n", val_host, ntohs(val_host));
	}
}

/* 1. t_nmap_cfg */
static void	print_dbg_cfg(const t_nmap_cfg *cfg)
{
	printf(ANSI_YELLOW "\n===================== [STRUCT: t_nmap_cfg] =====================\n" ANSI_RESET);
	dbg_str("target_name", cfg->target_name);
	dbg_str("ip", cfg->ip);
	dbg_str("file", cfg->file);
	dbg_str("spoof_ip", cfg->spoof_ip);

	dbg_int("num_ports", cfg->num_ports, cfg->num_ports <= 0);
	dbg_int("threads", cfg->threads, cfg->threads <= 0);
	dbg_int("ttl_value", cfg->ttl_value, cfg->ttl_value <= 0);
	dbg_int("os_detect", cfg->os_detect, 0);
	dbg_int("bad_sum", cfg->bad_sum, 0);
	dbg_int("packet_trace", cfg->packet_trace, 0);

	printf("  %-18s: ", "scan_time");
	if (cfg->scan_time <= 0.0)
		printf(ANSI_RED "[VOID / 0.0s]\n" ANSI_RESET);
	else
		printf(ANSI_BLUE "%.6f secs\n" ANSI_RESET, cfg->scan_time);

	printf("  %-18s: ", "scan_types");
	if (cfg->scan_types == 0)
		printf(ANSI_RED "[VOID / NO MASK]\n" ANSI_RESET);
	else
	{
		printf(ANSI_BLUE "0x%02X (" ANSI_RESET, cfg->scan_types);
		if (cfg->scan_types & SCAN_SYN)  printf(ANSI_BLUE "SYN " ANSI_RESET);
		if (cfg->scan_types & SCAN_NULL) printf(ANSI_BLUE "NULL " ANSI_RESET);
		if (cfg->scan_types & SCAN_FIN)  printf(ANSI_BLUE "FIN " ANSI_RESET);
		if (cfg->scan_types & SCAN_XMAS) printf(ANSI_BLUE "XMAS " ANSI_RESET);
		if (cfg->scan_types & SCAN_ACK)  printf(ANSI_BLUE "ACK " ANSI_RESET);
		if (cfg->scan_types & SCAN_UDP)  printf(ANSI_BLUE "UDP " ANSI_RESET);
		printf(ANSI_BLUE ")\n" ANSI_RESET);
	}

	/* Print the list of ports to scan (cfg.ports) */
	printf("  %-18s: ", "ports[] list");
	if (cfg->num_ports <= 0)
		printf(ANSI_RED "[VOID / NO PORTS CONFIGURED]\n" ANSI_RESET);
	else
	{
		int max_print = cfg->num_ports > 20 ? 20 : cfg->num_ports;
		printf(ANSI_BLUE "[ " ANSI_RESET);
		for (int i = 0; i < max_print; i++)
		{
			if (cfg->ports[i] <= 0 || cfg->ports[i] > 65535)
				printf(ANSI_RED "%d " ANSI_RESET, cfg->ports[i]);
			else
				printf(ANSI_BLUE "%d " ANSI_RESET, cfg->ports[i]);
		}
		if (cfg->num_ports > 20)
			printf(ANSI_CGRAY_LIGHT "... (+%d more) " ANSI_RESET, cfg->num_ports - 20);
		printf(ANSI_BLUE "]\n" ANSI_RESET);

		/* Endianness sample of the first port in memory */
		printf("  %-18s:\n", "ports[0] endianness");
		dbg_inspect_port_memory(&cfg->ports[0]);
	}
}

/* 2. t_pseudo_header */
static void	print_dbg_pseudo_header(const t_pseudo_header *ph)
{
	char	src_buf[INET_ADDRSTRLEN];
	char	dst_buf[INET_ADDRSTRLEN];

	printf(ANSI_YELLOW "\n===================== [STRUCT: t_pseudo_header] =====================\n" ANSI_RESET);
	
	inet_ntop(AF_INET, &ph->src_addr, src_buf, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &ph->dst_addr, dst_buf, INET_ADDRSTRLEN);

	printf("  %-18s: ", "src_addr");
	if (ph->src_addr == 0)
		printf(ANSI_RED "[VOID / 0.0.0.0]\n" ANSI_RESET);
	else
		printf(ANSI_BLUE "%s (0x%08X)\n" ANSI_RESET, src_buf, ph->src_addr);

	printf("  %-18s: ", "dst_addr");
	if (ph->dst_addr == 0)
		printf(ANSI_RED "[VOID / 0.0.0.0]\n" ANSI_RESET);
	else
		printf(ANSI_BLUE "%s (0x%08X)\n" ANSI_RESET, dst_buf, ph->dst_addr);

	dbg_int("placeholder", ph->placeholder, 0);
	dbg_int("protocol", ph->protocol, ph->protocol == 0);
	dbg_int("tcp_length", ph->tcp_length, ph->tcp_length == 0);
}

/* 3. t_port_report & t_port_state */
static void	print_dbg_port_reports(const t_map *map)
{
	const char	*scan_names[6] = {"SYN", "NULL", "FIN", "XMAS", "ACK", "UDP"};
	int			shown;
	int			port;
	int			s;

	printf(ANSI_YELLOW "\n===================== [STRUCT: t_port_report & t_port_state] =====================\n" ANSI_RESET);
	if (map->cfg.num_ports <= 0)
	{
		printf("  " ANSI_RED "[VOID: No ports to report]\n" ANSI_RESET);
		return ;
	}

	shown = map->cfg.num_ports > 10 ? 10 : map->cfg.num_ports;
	for (int i = 0; i < shown; i++)
	{
		port = map->cfg.ports[i];
		if (port < 0 || port > 65535)
			continue ;
		printf("  Port %-5d -> ", port);
		for (s = 0; s < 6; s++)
		{
			t_port_state st = map->results[port].states[s];
			if (st <= STATE_UNKNOWN || st >= MAX_PORT_STATES)
				printf("%s:" ANSI_RED "VOID " ANSI_RESET, scan_names[s]);
			else
				printf("%s:%s%s " ANSI_RESET, scan_names[s], g_state_colors[st], g_port_status[st]);
		}
		printf("\n");
	}
	if (map->cfg.num_ports > 10)
		printf("  ... (showing 10 of %d reported ports)\n", map->cfg.num_ports);
}

/* 4. t_map (Estructura principal) */
void	printer_debug(const char *tag, const t_map *map)
{
	if (!map)
	{
		printf(ANSI_RED "\n[DEBUG: %s] -> t_map is NULL\n" ANSI_RESET, tag ? tag : "UNKNOWN");
		return ;
	}

	printf(ANSI_MAGENTA "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("   DEBUG DUMP: %-42s \n", tag ? tag : "POINT");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" ANSI_RESET);

	print_dbg_cfg(&map->cfg);
	print_dbg_pseudo_header(&map->pseudo_header);
	print_dbg_port_reports(map);

	printf(ANSI_YELLOW "\n===================== [STRUCT: t_map (Engine Handles)] =====================\n" ANSI_RESET);
	dbg_ptr("pcap_handle", map->pcap_handle);
	dbg_ptr("addrinfo", map->addrinfo);
	dbg_int("socket_fd", map->socket_fd, map->socket_fd <= 0);
	dbg_int("detected_os_ttl", map->detected_os_ttl, map->detected_os_ttl == 0);

	printf(ANSI_MAGENTA "╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌── ⋆⋆ ╌╌\n\n" ANSI_RESET);
}