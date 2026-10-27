/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:05:50 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:05:52 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARCER_H
#define PARCER_H

#include "datatypes.h"

/* ** function prototypes ** */
//parser/parser_wrappers.c
int			handle_ip(char *arg, t_map *map);
int			handle_file(char *arg, t_map *map);
int			handle_ports(char *arg, t_map *map);
int			handle_speedup(char *arg, t_map *map);
int			handle_scan(char *arg, t_map *map);

//parser/parser_wrappers_bonus.c
int			handle_osdetect(char *arg, t_map *map);
int			handle_spoof(char *arg, t_map *map);
int			handle_badsum(char *arg, t_map *map);
int			handle_packettrace(char *arg, t_map *map);
int			handle_ttl(char *arg, t_map *map);

//parser/parse_args.c
int			parse_ip_arg(char *arg, t_nmap_cfg *cfg);
int			parse_file_arg(char *arg, t_nmap_cfg *cfg);
int			parse_speedup_arg(char *arg, t_nmap_cfg *cfg);
int			parse_scan_arg(char *arg, t_nmap_cfg *cfg);

//parser/parse_args_aux.c
int			is_valid_ip(const char *ip);
int			is_valid_port(const char *str);
int			get_scan_type(const char *str);
int			parse_int_range(const char *str, int min, int max);

//parser/parse_file.c
int			is_valid_hostname(const char *str);
int			is_numeric_address(const char *str);
int			validate_target_line(const char *line);
int			run_file_scans(t_map *map);

//parser/parse_ports.c
int			add_port(int port, t_nmap_cfg *cfg);
int			add_port_range(char *token, t_nmap_cfg *cfg);
int			parse_port_token(char *token, t_nmap_cfg *cfg);
int			parse_ports(const char *ports_str, t_nmap_cfg *cfg);
int			parse_ports_arg(char *arg, t_nmap_cfg *cfg);

//parser/parser.c
int 		validate_config(t_nmap_cfg *cfg);
//static const t_arg_map g_args[] = --- IGNORE ---
//static t_arg_map *find_arg(const char *key);
int			parse_args(int argc, char **argv, t_map *map);

#endif