/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 20:41:49 by agrumbac          #+#    #+#             */
/*   Updated: 2018/03/10 23:52:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline void	print_mem(const char *mem_state, const size_t size)
{
	size_t			i;
	size_t			j;

	i = 0;
	while (i < (MALLOC_ZONE * size - sizeof(t_malloc_mem)) / \
		(sizeof(t_malloc_chunk) + size))
	{
		if (!(i % 2))
			ft_printf("\n");
		j = 0;
		while (j < MALLOC_MINIMAP)
		{
			if (*(mem_state + i * MALLOC_MINIMAP + j) == '.')
				ft_printf("%s%c%s", "\e[42;30m", ' ', "\e[0m");
			else
				ft_printf("%s%c%s", "\e[41;34m", ' ', "\e[0m");
			j++;
		}
		i++;
	}
	ft_printf("\n");
}

static void			draw_mem(t_malloc_mem *mem, const size_t size)
{
	char			mem_state[MALLOC_ZONE * MALLOC_MINIMAP + 1];
	t_malloc_chunk	*chunk;
	int				chunk_pos;

	ft_printf("\n[Zone starting at %s%p%s of %s%lu%s bytes]",\
		"\e[33m", mem, "\e[32m", "\e[34m", size * MALLOC_ZONE, "\e[32m");
	ft_memset(mem_state, '.', MALLOC_ZONE * MALLOC_MINIMAP);
	mem_state[MALLOC_ZONE * MALLOC_MINIMAP] = '\0';
	chunk = mem->alloc;
	while (chunk)
	{
		chunk_pos = ((void*)chunk - ((void*)mem + sizeof(t_malloc_mem))) / \
			(size + sizeof(t_malloc_chunk));
		ft_memset((char*)mem_state + chunk_pos * MALLOC_MINIMAP, 'W', \
			chunk->size * MALLOC_MINIMAP / size);
		chunk = chunk->next;
	}
	print_mem((char *)mem_state, size);
}

void				show_alloc_mem_minimap(void)
{
	t_malloc_mem	*mem;

	pthread_mutex_lock(&g_malloc_mutex);
	if (g_malloc_zones.tiny)
	{
		ft_printf("%*sTINY ZONES: %s", MALLOC_MINIMAP, "\e[34;1m", "\e[0;32m");
		mem = g_malloc_zones.tiny;
		while (mem)
		{
			draw_mem(mem, ZONE_TINY);
			mem = mem->next;
		}
	}
	if (g_malloc_zones.small)
	{
		ft_printf("%*sSMALL ZONES: %s", MALLOC_MINIMAP, "\e[34;1m", "\e[0;32m");
		mem = g_malloc_zones.small;
		while (mem)
		{
			draw_mem(mem, ZONE_SMALL);
			mem = mem->next;
		}
	}
	ft_printf("\e[0m");
	pthread_mutex_unlock(&g_malloc_mutex);
}
