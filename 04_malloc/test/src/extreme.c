/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extreme.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 13:16:25 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/25 13:16:25 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/test.h"

static int	ranges_overlap(void *a, size_t a_size, void *b, size_t b_size)
{
	uintptr_t	a_start;
	uintptr_t	b_start;

	a_start = (uintptr_t)a;
	b_start = (uintptr_t)b;
	if (a_start > UINTPTR_MAX - a_size || b_start > UINTPTR_MAX - b_size)
		return (1);
	return (a_start < b_start + b_size && b_start < a_start + a_size);
}

static void	check_boundary_allocations(void)
{
	const size_t	sizes[] = {1, 15, 16, 17, 1024, 1025, 16384, 16385};
	const char		*labels[] = {
		"edge: size 1", "edge: size 15", "edge: size 16",
		"edge: size 17", "edge: size 1024", "edge: size 1025",
		"edge: size 16384", "edge: size 16385"
	};
	unsigned char	markers[8];
	unsigned char	*ptrs[8];
	size_t			i;
	size_t			j;
	int				disjoint;

	i = 0;
	while (i < sizeof(sizes) / sizeof(sizes[0]))
	{
		ptrs[i] = (unsigned char *)malloc(sizes[i]);
		markers[i] = (unsigned char)('A' + i);
		test_report_result(labels[i], ptrs[i] != NULL
			&& (uintptr_t)ptrs[i] % 16 == 0);
		disjoint = (ptrs[i] != NULL);
		j = 0;
		while (j < i && disjoint)
		{
			if (ptrs[j] == NULL
				|| ranges_overlap(ptrs[i], sizes[i], ptrs[j], sizes[j]))
				disjoint = 0;
			j++;
		}
		test_report_result("edge allocations do not overlap", disjoint);
		if (ptrs[i] != NULL)
		{
			ptrs[i][0] = markers[i];
			ptrs[i][sizes[i] - 1] = markers[i];
		}
		i++;
	}
	i = 0;
	while (i < sizeof(sizes) / sizeof(sizes[0]))
	{
		test_check_pointer_state(labels[i], ptrs[i], 1);
		if (ptrs[i] != NULL)
			test_report_result("edge payload remains intact",
				ptrs[i][0] == markers[i]
				&& ptrs[i][sizes[i] - 1] == markers[i]);
		free(ptrs[i]);
		i++;
	}
	i = 0;
	while (i < sizeof(sizes) / sizeof(sizes[0]))
	{
		test_check_pointer_state(labels[i], ptrs[i], 0);
		i++;
	}
}

static void	check_extreme_sizes(void)
{
	void			*ptr;
	volatile size_t	huge_size;

	ptr = malloc(0);
	test_report_result("malloc(0) returns NULL", ptr == NULL);
	free(ptr);
	huge_size = SIZE_MAX;
	ptr = malloc(huge_size);
	test_report_result("malloc(SIZE_MAX) returns NULL", ptr == NULL);
	free(ptr);
}

void	extreme_test(void)
{
	ft_putstr_fd(TEST_BANNER_EDGE, 1);
	check_extreme_sizes();
	check_boundary_allocations();
	print_stage("--- End of extreme test ---\n", C_TEST_EDGE);
}
