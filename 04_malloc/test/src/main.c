/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:49:04 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/26 10:39:02 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/test.h"

int	main(void)
{
	basic_test();
	write(1, "\n\n", 2);
	split_test();
	write(1, "\n\n", 2);
	extreme_test();
	write(1, "\n\n", 2);
	thread_test();
	test_print_summary();
	return (test_failure_count() != 0);
}
