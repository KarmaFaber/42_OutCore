/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:49:09 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:37:23 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/test.h"

void	basic_test(void)
{
	int				*num;
	unsigned char	*small_buf;
	unsigned char	*large_buf;

	ft_putstr_fd(TEST_BANNER_BASIC, 1);
	num = (int *)malloc(sizeof(int));
	if (num)
		*num = 42;
	small_buf = (unsigned char *)malloc(2048);
	if (small_buf)
		ft_memset(small_buf, 'S', 2048);
	large_buf = (unsigned char *)malloc(100000);
	if (large_buf)
		ft_memset(large_buf, 'L', 100000);

	test_check_pointer_state("basic: int allocation", num, 1);
	test_check_pointer_state("basic: small allocation", small_buf, 1);
	test_check_pointer_state("basic: large allocation", large_buf, 1);
	print_stage("--- After allocations ---\n", C_TEST_BASIC);
	print_sep();
	show_alloc_mem();
	print_sep();
	show_alloc_mem_ex();

	free(num);
	free(small_buf);
	free(large_buf);

	test_check_pointer_state("basic: int freed", num, 0);
	test_check_pointer_state("basic: small freed", small_buf, 0);
	test_check_pointer_state("basic: large freed", large_buf, 0);
	print_stage("--- After free ---\n", C_TEST_BASIC);
	print_sep();
	show_alloc_mem();
	print_sep();
	show_alloc_mem_ex();

	print_stage("--- End of basic test ---\n", C_TEST_BASIC);
}
