/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/18 17:34:16 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void			free_unused_mem(const int malloc_size, t_malloc_mem *mem)
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

	MALLOC_ULTRA_VERBOSE("%s[MUNMAP %p of %lu]%s", "\e[31m", mem, \
		MALLOC_ZONE * (malloc_size ? ZONE_SMALL : ZONE_TINY), "\e[0m");
	// unmap tiny small
	munmap(mem, MALLOC_ZONE * (malloc_size ? ZONE_SMALL : ZONE_TINY));
}

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
	// add to free list head
	chunk->prev = NULL;
	chunk->next = mem->free;
	if (mem->free)
		mem->free->prev = chunk;
	mem->free = chunk;
	// unmap if no more allocs
	if (!mem->alloc)
		free_unused_mem(malloc_size, mem);
}

static inline void	free_large(t_malloc_chunk *chunk)
{
	const size_t	msize = MALLOC_PAGE(chunk->size + sizeof(t_malloc_chunk));

	// remove from alloc list
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		g_malloc_zones.large = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;

	MALLOC_ULTRA_VERBOSE("%s[MUNMAP %p of %lu]%s", "\e[31m", chunk, msize, \
		"\e[0m");

	// unmap large
	munmap(chunk, msize);
}

static void			free_chunk(t_malloc_chunk *chunk)
{
	const int		malloc_size = MALLOC_SIZE(chunk->size);
	size_t const	zone_sizes[2] = {ZONE_TINY, ZONE_SMALL};
	void			*mem_zones[2] = {g_malloc_zones.tiny, g_malloc_zones.small};
	t_malloc_mem	*mem;

	if (malloc_size == MALLOC_LARGE)
		free_large(chunk);
	else
	{
		// find corresponding mem zone (safe because passed malloc_out_of_zones)
		mem = mem_zones[malloc_size];
		// while not in mem
		while (!((void *)chunk < (void *)mem + MALLOC_ZONE * \
			zone_sizes[malloc_size] && (void *)chunk > (void *)mem))
			mem = mem->next;
		free_tiny_small(chunk, malloc_size, mem);
	}
}

void				free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);

	MALLOC_ULTRA_VERBOSE("%s[free]%s of %s[%p]%s%s", "\e[32m", "\e[0m", \
		"\e[33m", ptr, "\e[0m", \
		malloc_out_of_zones(ptr) ? "\e[31m""[INVALID ADDR]""\e[0m" : "");
	if (!(!ptr || malloc_out_of_zones(ptr)))
		free_chunk(ptr - sizeof(t_malloc_chunk));
	MALLOC_ULTRA_VERBOSE("\n");
	pthread_mutex_unlock(&g_malloc_mutex);
}
