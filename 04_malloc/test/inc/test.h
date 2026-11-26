/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 10:47:37 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 13:25:42 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# define C_RESET       "\033[0m"
# define C_DIM         "\033[2m"
# define C_TEST_BASIC  "\033[1;34m"
# define C_TEST_SPLIT  "\033[1;35m"
# define C_TEST_EDGE   "\033[1;33m"
# define C_TEST_THREAD "\033[1;36m"
# define C_ZONE        "\033[1;36m"
# define C_TINY        "\033[0;32m"
# define C_SMALL       "\033[0;33m"
# define C_LARGE       "\033[0;35m"
# define C_FREE        "\033[1;32m"
# define C_USED        "\033[0;31m"
# define C_TEST_PASS   C_FREE
# define C_TEST_FAIL   C_USED

# define TEST_BANNER_BASIC \
	C_TEST_BASIC "=== BASIC TEST: TINY, SMALL & LARGE ===" C_RESET "\n"
# define TEST_BANNER_SPLIT \
	C_TEST_SPLIT "=== SPLIT TEST: TINY & SMALL ===" C_RESET "\n"
# define TEST_BANNER_EDGE \
	C_TEST_EDGE "=== EXTREME TEST: SIZE BOUNDARIES ===" C_RESET "\n"

#include "../../inc/malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Bypass Memcheck's malloc/free replacements to test this allocator directly. */
# define malloc ft_malloc
# define free ft_free

struct s_thread_args
{
	size_t	seed;
	int		failed;
};

void	debug_print_zones_chain(void);
void	debug_dump_state(void);

void	basic_test(void);
void	split_test(void);
void	extreme_test(void);
void	thread_test(void);
void	print_sep(void);
void	print_stage(const char *msg, const char *color);
void	test_check_pointer_state(const char *label, void *ptr,
			int should_be_allocated);
void	test_report_result(const char *label, int passed);
void	test_print_summary(void);
int		test_failure_count(void);

#endif
