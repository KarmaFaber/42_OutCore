/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:34 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:07:37 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	validate_config(t_nmap_cfg *cfg)
{
	int	i;

	if (!cfg->ip && !cfg->file)
		return (fprintf(stderr, ANSI_RED "Error: specify --ip or --file\n" ANSI_RESET), 1);
	if (cfg->ip && cfg->file)
		return (fprintf(stderr, ANSI_RED "Error: choose either --ip or --file\n" ANSI_RESET), 1);
	if (cfg->num_ports == 0)
	{
		i = 1;
		while (i <= 1024)
		{
			cfg->ports[cfg->num_ports++] = i;
			i++;
		}
	}
	if (cfg->scan_types == 0)
	{
		cfg->scan_types = SCAN_SYN | SCAN_NULL | SCAN_FIN \
						| SCAN_XMAS | SCAN_ACK | SCAN_UDP;
	}
	return (0);
}

static const t_arg_map g_args[] =
{
	{"--ip", 1, handle_ip},
	{"--file", 1, handle_file},
	{"--ports", 1, handle_ports},
	{"--speedup", 1, handle_speedup},
	{"--scan", 1, handle_scan},
	{"--os-detect", 0, handle_osdetect},        /* Modificado: con guion */
	{"--spoof-ip", 1, handle_spoof},           /* Renombrado para consistencia */
	{"--badsum", 0, handle_badsum},
	{"--packet-trace", 0, handle_packettrace},  /* Nuevo Bonus: Sin valor */
	{"--ttl", 1, handle_ttl},                   /* Nuevo Bonus: Requiere valor */
	{NULL, 0, NULL}
};

static t_arg_map	*find_arg(const char *key)
{
	int i = 0;

	while (g_args[i].key)
	{
		if (!strcmp(g_args[i].key, key))
			return ((t_arg_map *)&g_args[i]);
		i++;
	}
	return (NULL);
}

int parse_args(int argc, char **argv, t_map *map)
{
	int			i;
	t_arg_map	*arg;

	i = 1;
	while (i < argc)
	{
		if (!strcmp(argv[i], "--help"))
			return (print_usage(), exit(0), 0);
		arg = find_arg(argv[i]);
		if (!arg || (arg->needs_value && i + 1 >= argc))
			return (print_usage(), 1);
		if (arg->needs_value && arg->handler(argv[++i], map))
			return (1);
		if (!arg->needs_value && arg->handler(NULL, map))
			return (1);
		i++;
	}
	return (validate_config(&map->cfg));
}
