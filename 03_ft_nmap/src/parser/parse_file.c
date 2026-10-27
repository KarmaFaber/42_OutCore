/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:06:52 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:06:55 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	is_valid_hostname(const char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if ((str[i] >= 'a' && str[i] <= 'z') || \
			(str[i] >= 'A' && str[i] <= 'Z') || \
			(str[i] >= '0' && str[i] <= '9') || \
			str[i] == '.' || str[i] == '-')
		{
			i++;
		}
		else
			return (0);
	}
	return (1);
}

int	is_numeric_address(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
			return (0);
		i++;
	}
	return (1);
}

int	validate_target_line(const char *line)
{
	if (is_numeric_address(line))
	{
		if (!is_valid_ip(line))
		{
			fprintf(stderr, ANSI_RED "\nWarning: Skipping invalid IP '%s'\n" ANSI_RESET, line);
			return (0);
		}
	}
	else if (!is_valid_hostname(line))
	{
		fprintf(stderr, ANSI_RED "\nWarning: Skipping invalid hostname '%s'\n" ANSI_RESET, line);
		return (0);
	}
	return (1);
}

int	run_file_scans(t_map *map)
{
	FILE	*fp;
	char	line[256];
	size_t	len;

	fp = fopen(map->cfg.file, "r");
	if (!fp)
		return (fprintf(stderr, ANSI_RED "Error: Cannot open targets file\n" ANSI_RESET), 1);
	while (fgets(line, sizeof(line), fp))
	{
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (len > 1 && line[len - 2] == '\r')
			line[len - 2] = '\0';
		if (strlen(line) == 0 || line[0] == '#')
			continue ;
		if (validate_target_line(line))
			execute_scan_for_target(line, map);
	}
	fclose(fp);
	return (0);
}
