/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:59:14 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:39:46 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/test.h"

static int	g_test_failures;

static void	write_text(const char *text)
{
	(void)write(1, text, strlen(text));
}

static void	report_result(const char *label, int passed, const char *detail)
{
	if (passed)
		write_text(C_TEST_PASS "[PASS] ");
	else
	{
		write_text(C_TEST_FAIL "[FAIL] ");
		g_test_failures++;
	}
	write_text(label);
	if (!passed && detail)
	{
		write_text(": ");
		write_text(detail);
	}
	write_text(C_RESET "\n");
}

static int	zone_accounting_valid(t_zone *zone)
{
	t_block	*block;
	size_t	offset;
	size_t	free_sum;
	size_t	block_count;

	if (zone->total_size < sizeof(t_zone) + sizeof(t_block))
		return (0);
	offset = sizeof(t_zone);
	free_sum = 0;
	block_count = 0;
	while (block_count++ < zone->total_size / sizeof(t_block))
	{
		if (offset > zone->total_size - sizeof(t_block))
			return (0);
		block = (t_block *)((char *)zone + offset);
		if (block->is_free != 0 && block->is_free != 1)
			return (0);
		if (block->is_free)
		{
			if (SIZE_MAX - free_sum < block->size)
				return (0);
			free_sum += block->size;
		}
		if (block->next_offset == 0)
			return (free_sum == zone->free_size
				&& zone->free_size <= zone->total_size);
		if (block->next_offset <= offset
			|| block->next_offset > zone->total_size - sizeof(t_block))
			return (0);
		offset = block->next_offset;
	}
	return (0);
}

static t_zone	*find_pointer_zone(void *ptr, int *valid_list)
{
	t_zone	*zone;
	uintptr_t	addr;
	uintptr_t	start;
	size_t	i;

	zone = g_zones;
	addr = (uintptr_t)ptr;
	i = 0;
	while (zone && i < MAX_ZONES)
	{
		start = (uintptr_t)zone;
		if (zone->total_size <= UINTPTR_MAX - start
			&& addr >= start && addr < start + zone->total_size)
			return (zone);
		zone = zone->next;
		i++;
	}
	if (zone)
		*valid_list = 0;
	return (NULL);
}

static t_block	*find_pointer_block(t_zone *zone, void *ptr)
{
	t_block	*block;
	size_t	offset;
	void	*payload;

	offset = sizeof(t_zone);
	while (offset <= zone->total_size - sizeof(t_block))
	{
		block = (t_block *)((char *)zone + offset);
		payload = (void *)ALIGN_16((uintptr_t)((char *)block
					+ sizeof(t_block)));
		if (payload == ptr)
			return (block);
		if (block->next_offset == 0
			|| block->next_offset <= offset
			|| block->next_offset > zone->total_size - sizeof(t_block))
			return (NULL);
		offset = block->next_offset;
	}
	return (NULL);
}

void	test_report_result(const char *label, int passed)
{
	report_result(label, passed, NULL);
}

void	test_print_summary(void)
{
	if (g_test_failures == 0)
		write_text(C_TEST_PASS "[PASS] All allocator checks passed"
			C_RESET "\n");
	else
		write_text(C_TEST_FAIL
			"[FAIL] Allocator checks failed; see red diagnostics above"
			C_RESET "\n");
}

int	test_failure_count(void)
{
	return (g_test_failures);
}

void	test_check_pointer_state(const char *label, void *ptr,
		int should_be_allocated)
{
	t_zone	*zone;
	t_block	*block;
	int		valid_list;

	valid_list = 1;
	if (ptr == NULL)
	{
		report_result(label, !should_be_allocated,
			"malloc returned NULL");
		return ;
	}
	zone = find_pointer_zone(ptr, &valid_list);
	if (!valid_list)
	{
		report_result(label, 0, "zone list is invalid");
		return ;
	}
	if (zone == NULL)
	{
		report_result(label, !should_be_allocated,
			"pointer is not in an allocated zone");
		return ;
	}
	if (!zone_accounting_valid(zone))
	{
		report_result(label, 0,
			"zone free_size differs from its free blocks");
		return ;
	}
	block = find_pointer_block(zone, ptr);
	if (block == NULL)
	{
		report_result(label, 0, "pointer has no matching block");
		return ;
	}
	if (zone->group == LARGE && !should_be_allocated)
	{
		report_result(label, 0, "freed large zone is still linked");
		return ;
	}
	report_result(label, block->is_free == !should_be_allocated,
		"block state does not match allocation/free");
}

void	print_sep(void)
{
	ft_putstr_fd(C_DIM, 1);
	ft_putstr_fd("--------------------------------------------------\n", 1);
	ft_putstr_fd(C_RESET, 1);
}


void	print_stage(const char *msg, const char *color)
{
	write_text(color);
	write_text(msg);
	write_text(C_RESET);
}
