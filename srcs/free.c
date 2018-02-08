/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/08 00:48:44 by agrumbac         ###   ########.fr       */
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
			MALLOC_ZONE * FREE_SIZE(malloc_size), "\e[31m");//
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
		MALLOC_PAGE(chunk->size + sizeof(t_malloc_chunk)), "\e[31m");//
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

static int			out_of_bounds(const void *ptr)
{
	// TODO find a way to know if addr is on page!
	// TODO check if in malloced zone

	return (!!ptr);//always true
}

void			free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("%s[free]%s of %s[%p]%s", "\e[32m", "\e[0m", "\e[33m", ptr, "\e[0m");
	#endif
	if (!(!ptr || out_of_bounds(ptr)))
		free_chunk(ptr - sizeof(t_malloc_chunk));
	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("\n");
	#endif
	pthread_mutex_unlock(&g_malloc_mutex);
}
