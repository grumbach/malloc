/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 07:09:36 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/16 19:42:09 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline int	is_not_in_chunks(const void *ptr, t_malloc_chunk *chunk)
{
	while (chunk)
	{
		//if in chunk
		if (ptr >= (void*)chunk && \
			ptr <= (void*)chunk + sizeof(t_malloc_chunk) + chunk->size)
		{
			if ((void*)chunk + sizeof(t_malloc_chunk) == ptr)
				return (0);//end OK
			return (1);//end KO
		}
		chunk = chunk->next;
	}
	return (2);//continue
}

int					malloc_out_of_zones(const void *ptr)
{
	const size_t	zone_sizes[3] = {ZONE_TINY, ZONE_SMALL, 0};
	t_malloc_mem	*mem;
	int				pos;
	int				i;

	if (!((pos = is_not_in_chunks(ptr, g_malloc_zones.large)) & 2))
		return (pos);
	i = -1;
	while (zone_sizes[++i])
	{
		mem = i ? g_malloc_zones.small : g_malloc_zones.tiny;
		while (mem)
		{
			if (ptr >= (void*)mem && \
				ptr <= (void*)mem + MALLOC_ZONE * zone_sizes[i])
			{
				if (!((pos = is_not_in_chunks(ptr, mem->alloc)) & 2))
					return (pos);
				return (1);
			}
			mem = mem->next;
		}
	}
	return (1);
}

static inline void	show_alloc_tiny_small(t_malloc_mem *mem, size_t *total, \
						const char *size_str)
{
	t_malloc_chunk	*chunk;

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

static inline void	show_alloc_large(size_t *total)
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

void				show_alloc_mem(void)
{
	size_t			total;

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

void				show_alloc_mem_hex(void *ptr)
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
