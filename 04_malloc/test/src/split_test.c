/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:53:55 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:39:13 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/test.h"

static void	fill_pattern(unsigned char *buf, size_t len, unsigned char val)
{
	size_t	i;

	if (!buf)
		return ;
	i = 0;
	while (i < len)
	{
		buf[i] = val;
		i++;
	}
}

void	split_test(void)
{
	unsigned char	*t1;
	unsigned char	*t2;
	unsigned char	*t3;
	unsigned char	*s1;
	unsigned char	*s2;

	ft_putstr_fd(TEST_BANNER_SPLIT, 1);
	t1 = (unsigned char *)malloc(16);
	t2 = (unsigned char *)malloc(32);
	t3 = (unsigned char *)malloc(48);
	fill_pattern(t1, 16, 'A');
	fill_pattern(t2, 32, 'B');
	fill_pattern(t3, 48, 'C');

	s1 = (unsigned char *)malloc(2048);
	s2 = (unsigned char *)malloc(4096);
	fill_pattern(s1, 2048, 'X');
	fill_pattern(s2, 4096, 'Y');

	test_check_pointer_state("split: t1 allocated", t1, 1);
	test_check_pointer_state("split: t2 allocated", t2, 1);
	test_check_pointer_state("split: t3 allocated", t3, 1);
	test_check_pointer_state("split: s1 allocated", s1, 1);
	test_check_pointer_state("split: s2 allocated", s2, 1);
	print_stage("--- After allocations ---\n", C_TEST_SPLIT);
	print_sep();
	show_alloc_mem();
	print_sep();
	show_alloc_mem_ex();

	free(t2);
	free(s1);

	test_check_pointer_state("split: t2 freed", t2, 0);
	test_check_pointer_state("split: s1 freed", s1, 0);
	test_check_pointer_state("split: t1 remains allocated", t1, 1);
	test_check_pointer_state("split: t3 remains allocated", t3, 1);
	test_check_pointer_state("split: s2 remains allocated", s2, 1);
	print_stage("--- After freeing t2 (TINY) and s1 (SMALL) ---\n",
		C_TEST_SPLIT);
	print_sep();
	show_alloc_mem();
	print_sep();
	show_alloc_mem_ex();

	free(t1);
	free(t3);
	free(s2);

	test_check_pointer_state("split: t1 freed", t1, 0);
	test_check_pointer_state("split: t3 freed", t3, 0);
	test_check_pointer_state("split: s2 freed", s2, 0);
	print_stage("--- After freeing remaining blocks ---\n", C_TEST_SPLIT);
	print_sep();
	show_alloc_mem_ex();
	print_stage("--- End of split test ---\n", C_TEST_SPLIT);
}
