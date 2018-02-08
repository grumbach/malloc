/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 07:09:36 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/06 23:59:57 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline void show_alloc_tiny_small(t_malloc_mem *mem, size_t *total, \
						const char *size_str)
{
	t_malloc_chunk		*chunk;

	while (mem)
	{
		ft_printf("%s : %p\n", size_str, mem);
		chunk = mem->alloc;
		while (chunk)
		{
			ft_printf("%p - %p : %lu bytes\n", chunk + sizeof(t_malloc_chunk), \
				chunk + sizeof(t_malloc_chunk) + chunk->size, chunk->size);
			*total += chunk->size;
			chunk = chunk->next;
		}
		mem = mem->next;
	}
}

static void	show_alloc_tiny(size_t *total)
{
	if (!g_malloc_zones.tiny)
		return ;
	show_alloc_tiny_small(g_malloc_zones.tiny, total, "TINY");
}

static void	show_alloc_small(size_t *total)
{
	if (!g_malloc_zones.small)
		return ;
	show_alloc_tiny_small(g_malloc_zones.small, total, "SMALL");
}

static inline void	show_alloc_large(size_t *total)
{
	t_malloc_chunk		*chunk;

	if (!g_malloc_zones.large)
		return ;
	ft_printf("LARGE : %p\n", g_malloc_zones.large);
	chunk = g_malloc_zones.large;
	while (chunk)
	{
		ft_printf("%p - %p : %lu bytes\n", chunk + sizeof(t_malloc_chunk), \
			chunk + sizeof(t_malloc_chunk) + chunk->size, chunk->size);
		*total += chunk->size;
		chunk = chunk->next;
	}
}

void		show_alloc_mem()
{
	size_t	total;

	pthread_mutex_lock(&g_malloc_mutex);
	total = 0;
	show_alloc_tiny(&total);
	show_alloc_small(&total);
	show_alloc_large(&total);
	ft_printf("Total : %lu bytes\n", total);
	pthread_mutex_unlock(&g_malloc_mutex);
}

void		show_alloc_mem_hex(void *ptr)
{
	(void)ptr;
	ft_printf("hello from show_alloc");
}

/*
** TINY : 0xA0000
** 0xA0020 - 0xA004A : 42 bytes
** 0xA006A - 0xA00BE : 84 bytes
** SMALL : 0xAD000
** 0xAD020 - 0xADEAD : 3725 bytes
** LARGE : 0xB0000
** 0xB0020 - 0xBBEEF : 48847 bytes
** Total : 52698 bytes
*/
