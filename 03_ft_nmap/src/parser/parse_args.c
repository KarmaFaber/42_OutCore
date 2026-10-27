/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:06:44 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:06:47 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int parse_ip_arg(char *arg, t_nmap_cfg *cfg)
{
	if (!is_valid_ip(arg))
	{
		fprintf(stderr, ANSI_RED "Error: Invalid IP\n" ANSI_RESET);
		return (1);
	}
	cfg->ip = arg;
	return (0);
}

int parse_file_arg(char *arg, t_nmap_cfg *cfg)
{
	FILE	*fp;

	fp = fopen(arg, "r");
	if (!fp)
	{
		fprintf(stderr, ANSI_RED "Error: Cannot open file '%s': %s\n" ANSI_RESET, \
				arg, strerror(errno));
		return (1);
	}
	fclose(fp);
	cfg->file = arg;
	return (0);
}

int parse_speedup_arg(char *arg, t_nmap_cfg *cfg)
{
	cfg->threads = parse_int_range(arg, 0, 250);

	if (cfg->threads == -1)
	{
		fprintf(stderr, ANSI_RED
			"Error: Threads must be 0-250\n" ANSI_RESET);
		return (1);
	}
	return (0);
}

int parse_scan_arg(char *arg, t_nmap_cfg *cfg)
{
	char	*copy;
	char	*token;
	int	 scan;

	copy = strdup(arg);
	if (!copy)
		return (1);
	token = strtok(copy, ",");
	while (token)
	{
		scan = get_scan_type(token);

		if (!scan)
		{
			free(copy);
			fprintf(stderr,
				ANSI_RED "Error: Invalid scan type\n" ANSI_RESET);
			return (1);
		}
		cfg->scan_types |= scan;
		token = strtok(NULL, ",");
	}
	free(copy);
	return (0);
}
