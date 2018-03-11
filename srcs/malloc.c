/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:26 by agrumbac          #+#    #+#             */
/*   Updated: 2018/03/11 18:14:28 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_malloc_zones		g_malloc_zones = {0, 0, 0};
pthread_mutex_t		g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
** give_chunk()
** makes first free chunk : first in alloc list
** returns malloced address
*/

static inline void	*give_chunk(t_malloc_chunk **free, t_malloc_chunk **alloc, \
					const size_t size)
{
	t_malloc_chunk	*chunk;

	chunk = *free;
	(*free) = chunk->next;
	if ((*free))
		(*free)->prev = NULL;
	chunk->next = *alloc;
	if ((*alloc))
		(*alloc)->prev = chunk;
	(*alloc) = chunk;
	chunk->size = size;
	return (chunk + 1);
}

/*
** mem_init_zone()
** initializes memory zone :
** puts newly mmaped mem as the first in the malloc_zone
** creates a chained-list of free chunks with with size : size
*/

static inline void	mem_init_zone(t_malloc_mem **malloc_zone, \
					t_malloc_mem *mem, const size_t zone_size)
{
	t_malloc_chunk	*free_chunk;

	free_chunk = (void*)mem + sizeof(t_malloc_mem);
	mem->prev = NULL;
	mem->next = *malloc_zone;
	*malloc_zone = mem;
	if (mem->next)
		mem->next->prev = mem;
	mem->alloc = NULL;
	mem->free = free_chunk;
	while ((void*)free_chunk + (zone_size + sizeof(t_malloc_chunk)) * 2 < \
		(void*)mem + zone_size * MALLOC_ZONE)
	{
		free_chunk->next = (void*)free_chunk + \
			sizeof(t_malloc_chunk) + zone_size;
		free_chunk->next->prev = free_chunk;
		free_chunk = free_chunk->next;
	}
	free_chunk->next = NULL;
}

static inline void	*malloc_tiny_small(t_malloc_mem **malloc_zone, \
					const size_t zone_size, const size_t size)
{
	t_malloc_mem	*mem;

	mem = *malloc_zone;
	while (mem && !mem->free)
		mem = mem->next;
	if (!mem)
	{
		mem = mmap(0, zone_size * MALLOC_ZONE, \
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		MALLOC_ULTRA_VERBOSE("%s[MMAP %p]%s", "\e[31m", mem, "\e[0m");
		if (mem == MAP_FAILED)
			return (NULL);
		mem_init_zone(malloc_zone, mem, zone_size);
	}
	return (give_chunk(&mem->free, &mem->alloc, size));
}

static void			*malloc_large(size_t size)
{
	const size_t	msize = MALLOC_PAGE(size + sizeof(t_malloc_chunk));
	t_malloc_chunk	*ptr;

	MALLOC_ULTRA_VERBOSE("[large] of %s%lu%s\t", "\e[33m", size, "\e[0m");
	ptr = mmap(0, msize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	MALLOC_ULTRA_VERBOSE("%s[MMAP %p of %lu]%s", "\e[31m", ptr, msize, "\e[0m");
	if (ptr == MAP_FAILED)
		return (NULL);
	ptr->next = g_malloc_zones.large;
	ptr->prev = NULL;
	ptr->size = size;
	if (g_malloc_zones.large)
		g_malloc_zones.large->prev = ptr;
	g_malloc_zones.large = ptr;
	return ((void*)ptr + sizeof(t_malloc_chunk));
}

void				*malloc(size_t size)
{
	const int		malloc_size = MALLOC_SIZE(size);
	void			*ptr;

	if (!size || size > MALLOC_MAX)
		return (NULL);
	pthread_mutex_lock(&g_malloc_mutex);
	if (malloc_size == MALLOC_TINY)
	{
		MALLOC_ULTRA_VERBOSE("[tiny] of %s%lu%s\t", "\e[33m", size, "\e[0m");
		ptr = malloc_tiny_small(&g_malloc_zones.tiny, ZONE_TINY, size);
	}
	else if (malloc_size == MALLOC_SMALL)
	{
		MALLOC_ULTRA_VERBOSE("[small] of %s%lu%s\t", "\e[33m", size, "\e[0m");
		ptr = malloc_tiny_small(&g_malloc_zones.small, ZONE_SMALL, size);
	}
	else
	{
		ptr = malloc_large(size);
	}
	MALLOC_ULTRA_VERBOSE(" %s[%p]%s\n", "\e[33m", ptr, "\e[0m");
	pthread_mutex_unlock(&g_malloc_mutex);
	return (ptr);
}
