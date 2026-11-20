/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 10:47:37 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/20 14:36:38 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

#define C_RESET "\033[0m"
#define C_ZONE "\033[1;36m"
#define C_TINY "\033[1;32m"
#define C_SMALL "\033[1;33m"
#define C_LARGE "\033[1;35m"
#define C_FREE "\033[0;32m"
#define C_USED "\033[0;31m"

#include "../../inc/malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//debugs:

void	debug_print_zones_chain(void);
void	debug_dump_state(void);


#endif
