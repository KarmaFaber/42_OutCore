/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:05:57 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/27 11:05:59 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTER_H
#define PRINTER_H

#include "datatypes.h"

//printers/print_usage.c
void		print_usage(void);

//printers/print_init.c
void		print_scan_types(int scan_types);
void		print_bonus_intro(t_nmap_cfg *cfg);
void		print_scan_intro(t_nmap_cfg *cfg);

//printers/print_result.c
void		print_scan_results( t_map *map, int port, t_port_state state);

#endif