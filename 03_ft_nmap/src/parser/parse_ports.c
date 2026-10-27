/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ports.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:07:03 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 14:14:07 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

/**
 * @brief Adds a port to the configuration if it is not already present.
 *
 * @param port Port number to add.
 * @param cfg Pointer to the Nmap configuration.
 * @return 0 on success, 1 if the port is invalid or the limit is reached.
 */
int add_port(int port, t_nmap_cfg *cfg)
{
	int	i;

	if (port < 0 || port > 65535)
		return (1);

	i = 0;
	while (i < cfg->num_ports)
	{
		if (cfg->ports[i] == port)
			return (0); // Port already exists, do not add
		i++;
	}
	if (cfg->num_ports >= MAX_PORTS)
		return (1);

	cfg->ports[cfg->num_ports++] = port;
	return (0);
}

/**
 * @brief Parses a port range and adds each port to the configuration.
 *
 * @param token String containing the port range.
 * @param cfg Pointer to the Nmap configuration.
 * @return 0 on success, 1 if the range is invalid.
 */
int	add_port_range(char *token, t_nmap_cfg *cfg)
{
	char	*dash;
	int		start;
	int		end;

	dash = strchr(token, '-');
	if (!dash || dash == token || *(dash + 1) == '\0')
		return (1);
	*dash = '\0';
	start = is_valid_port(token);
	end = is_valid_port(dash + 1);
	*dash = '-';
	if (start == -1 || end == -1 || start > end)
		return (1);
	while (start <= end)
	{
		if (add_port(start, cfg))
			return (1);
		start++;
	}
	return (0);
}

/**
 * @brief Parses a port number or range and adds it to the configuration.
 *
 * @param token String containing a port number or range.
 * @param cfg Pointer to the Nmap configuration.
 * @return 0 on success, 1 if the token is invalid.
 */
int parse_port_token(char *token, t_nmap_cfg *cfg)
{
	int port;

	if (strchr(token, '-'))
		return (add_port_range(token, cfg));
	port = is_valid_port(token);
	if (port == -1)
		return (1);
	return (add_port(port, cfg));
}

/**
 * @brief Parses a comma-separated list of ports and ranges.
 *
 * @param ports_str String containing the port specifications.
 * @param cfg Pointer to the Nmap configuration.
 * @return 0 on success, 1 if parsing fails.
 */
int parse_ports(const char *ports_str, t_nmap_cfg *cfg)
{
	char	*copy;
	char	*token;

	copy = strdup(ports_str);
	if (!copy)
		return (1);

	token = strtok(copy, ",");

	while (token)
	{
		if (parse_port_token(token, cfg))
			return (free(copy), 1);

		token = strtok(NULL, ",");
	}

	free(copy);
	return (0);
}

/**
 * @brief Parses the port argument and reports errors if invalid.
 *
 * @param arg String containing the port specifications.
 * @param cfg Pointer to the Nmap configuration.
 * @return 0 on success, 1 on error.
 */
int parse_ports_arg(char *arg, t_nmap_cfg *cfg)
{
	if (parse_ports(arg, cfg))
	{
		fprintf(stderr,
			 ANSI_RED "Error: Invalid port specification\n" ANSI_RESET);
		return (1);
	}
	return (0);
}
