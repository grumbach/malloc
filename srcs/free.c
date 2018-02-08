/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/09 21:42:21 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline void	free_tiny_small(t_malloc_chunk *chunk, \
						const int malloc_size, t_malloc_mem *mem)
{
	// remove from alloc list
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		mem->alloc = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;
	// add to free list if tiny or small and unmap tiny small
	chunk->prev = NULL;
	chunk->next = mem->free;
	mem->free = chunk;
	// unmap if no more allocs
	if (!mem->alloc)
	{
		// remove form list
		if (mem->prev)
			mem->prev->next = mem->next;
		else if (malloc_size)
			g_malloc_zones.small = mem->next;
		else
			g_malloc_zones.tiny = mem->next;
		if (mem->next)
			mem->next->prev = mem->prev;

		#ifdef MALLOC_DEBUG_VERBOSE
		ft_printf("%s[MUNMAP %p of %lu]%s", "\e[31m", mem, \
			MALLOC_ZONE * FREE_SIZE(malloc_size), "\e[0m");//
		#endif
		// unmap tiny small
		munmap(mem, MALLOC_ZONE * FREE_SIZE(malloc_size));
	}
}

static inline void	free_large(t_malloc_chunk *chunk)
{
	// remove from alloc list
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		g_malloc_zones.large = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("%s[MUNMAP %p of %lu]%s", "\e[31m", chunk, chunk->size + \
		sizeof(t_malloc_chunk) + \
		MALLOC_PAGE(chunk->size + sizeof(t_malloc_chunk)), "\e[0m");//
	#endif

	// unmap large
	munmap(chunk, chunk->size + sizeof(t_malloc_chunk) + \
		MALLOC_PAGE(chunk->size + sizeof(t_malloc_chunk)));
}

static void			free_chunk(t_malloc_chunk *chunk)
{
	const int			malloc_size = MALLOC_SIZE(chunk->size);
	void				*mem = &g_malloc_zones + malloc_size;

	if (malloc_size >> 1)
		free_large(chunk);
	free_tiny_small(chunk, malloc_size, mem);
}

static inline int	is_not_in_chunks(const void *ptr, t_malloc_chunk *chunk)
{
	while (chunk)
	{
		//if in chunk
		if (ptr > (void*)chunk && \
			ptr < (void*)chunk + sizeof(t_malloc_chunk) + chunk->size)
		{
			if (chunk + 1 == ptr)
				return (0);//end OK
			return (1);//end KO
		}
		chunk = chunk->next;
	}
	return (2);//continue
}

static int			out_of_zones(const void *ptr)
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
			if (ptr > (void*)mem && \
				ptr < (void*)mem + MALLOC_ZONE * zone_sizes[i])
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

void			free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("%s[free]%s of %s[%p]%s", "\e[32m", "\e[0m", "\e[33m", ptr, "\e[0m");
	ft_printf("%s", out_of_zones(ptr) ? "\e[31m""[INVALID ADDR]""\e[0m" : "");
	#endif
	if (!(!ptr || out_of_zones(ptr)))
		free_chunk(ptr - sizeof(t_malloc_chunk));
	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("\n");
	#endif
	pthread_mutex_unlock(&g_malloc_mutex);
}
