/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 07:09:36 by agrumbac          #+#    #+#             */
/*   Updated: 2018/03/08 20:36:25 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline void		show_alloc_tiny_small(t_malloc_mem *mem, \
						size_t *total, const char *size_str)
{
	t_malloc_chunk		*chunk;

	while (mem)
	{
		ft_printf("%s : %p\n", size_str, mem);
		chunk = mem->alloc;
		while (chunk)
		{
			ft_printf("%p - %p : %lu bytes\n", (void *)chunk + \
			sizeof(t_malloc_chunk), (void *)chunk + sizeof(t_malloc_chunk) + \
			chunk->size, chunk->size);
			*total += chunk->size;
			chunk = chunk->next;
		}
		mem = mem->next;
	}
}

static inline void		show_alloc_large(size_t *total)
{
	t_malloc_chunk	*chunk;

	ft_printf("LARGE : %p\n", g_malloc_zones.large);
	chunk = g_malloc_zones.large;
	while (chunk)
	{
		ft_printf("%p - %p : %lu bytes\n", (void *)chunk + \
		sizeof(t_malloc_chunk), (void *)chunk + sizeof(t_malloc_chunk) + \
		chunk->size, chunk->size);
		*total += chunk->size;
		chunk = chunk->next;
	}
}

void					show_alloc_mem(void)
{
	size_t				total;

	total = 0;
	pthread_mutex_lock(&g_malloc_mutex);
	if (g_malloc_zones.tiny)
		show_alloc_tiny_small(g_malloc_zones.tiny, &total, "TINY");
	if (g_malloc_zones.small)
		show_alloc_tiny_small(g_malloc_zones.small, &total, "SMALL");
	if (g_malloc_zones.large)
		show_alloc_large(&total);
	ft_printf("Total : %lu bytes\n", total);
	pthread_mutex_unlock(&g_malloc_mutex);
}

static inline size_t	dump_line(const char *ptr, const size_t size)
{
	size_t				i;

	ft_printf("%.16p: ", ptr);
	i = 0;
	while (i < 16)
	{
		if (!(i % 2))
			ft_printf(" ");
		if (size <= i)
			ft_printf("  ");
		if (size > i)
			ft_printf("%02x", ptr[i]);
		i++;
	}
	ft_printf("  ");
	i = 0;
	while (i < 16 && size > i)
	{
		if (ft_isprint(ptr[i]))
			ft_printf("%c", ptr[i]);
		else
			ft_printf(".");
		i++;
	}
	return (i);
}

void					show_alloc_mem_hex(void *ptr)
{
	const char			*zone_str[3] = {"Tiny", "Small", "Large"};
	size_t				size;
	size_t				i;

	pthread_mutex_lock(&g_malloc_mutex);
	if (ptr && !malloc_out_of_zones(ptr))
	{
		size = ((t_malloc_chunk *)(ptr - sizeof(t_malloc_chunk)))->size;
		ft_printf("%s memory area of %lu bytes starting at %p:\n", \
			zone_str[MALLOC_SIZE(size)], size, ptr);
		while (size)
		{
			i = dump_line(ptr, size);
			size -= i;
			ptr += i;
			ft_printf("\n");
		}
	}
	else
		ft_printf("Memory address [%p] was not allocated by malloc\n", ptr);
	pthread_mutex_unlock(&g_malloc_mutex);
}
