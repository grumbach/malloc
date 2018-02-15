/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:26 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/15 07:17:58 by agrumbac         ###   ########.fr       */
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
	//remove chunk from free-list
	(*free) = chunk->next;
	if ((*free))
		(*free)->prev = NULL;
	//now add to alloc-list head
	chunk->next = *alloc;
	if ((*alloc))
		(*alloc)->prev = chunk;
	(*alloc) = chunk;
	//chunk was first of free list so prev is already NULL

	//set chunk size
	chunk->size = size;
	return (chunk + 1);//check this ptr
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
	t_malloc_chunk	*free_chunk = (void*)mem + sizeof(t_malloc_mem);
	//set as first mem
	mem->prev = NULL;
	mem->next = *malloc_zone;
	*malloc_zone = mem;
	if (mem->next)
		mem->next->prev = mem;
	//there are no allocs yet ;)
	mem->alloc = NULL;
	//string a beautiful chain of free chunks!
	mem->free = free_chunk;
	// while there is room for another chunk (t_malloc_chunk + zone_size) in mem
	while ((void*)free_chunk + (zone_size + sizeof(t_malloc_chunk)) * 2 < \
		(void*)mem + zone_size * MALLOC_ZONE)
	{
		//make new as next
		free_chunk->next = (void*)free_chunk + sizeof(t_malloc_chunk) + zone_size;
		//link both
		free_chunk->next->prev = free_chunk;
		//become next
		free_chunk = free_chunk->next;
	}
	// ->[]<->[]<->[]->NULL
	free_chunk->next = NULL;
}

static inline void	*malloc_tiny_small(t_malloc_mem **malloc_zone, \
					const size_t zone_size, const size_t size)
{
	t_malloc_mem	*mem;

	//check for mem
	mem = *malloc_zone;
	// while no free memory in mem
	while (mem && !mem->free)
		mem = mem->next;
	//if no memory or no free memory
	if (!mem)
	{
		mem = mmap(0, zone_size * MALLOC_ZONE, \
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		MALLOC_ULTRA_VERBOSE("%s[MMAP %p]%s", "\e[31m", mem, "\e[0m");
		if (mem == MAP_FAILED)
			return (NULL);
		mem_init_zone(malloc_zone, mem, zone_size);
	}
	//mem available t_malloc_mem
	return (give_chunk(&mem->free, &mem->alloc, size));
}

static void			*malloc_tiny(size_t size)
{
	MALLOC_ULTRA_VERBOSE("[tiny] of %s%lu%s\t", "\e[33m", size, "\e[0m");
	return (malloc_tiny_small(&g_malloc_zones.tiny, ZONE_TINY, size));
}

static void			*malloc_small(size_t size)
{
	MALLOC_ULTRA_VERBOSE("[small] of %s%lu%s\t", "\e[33m", size, "\e[0m");
	return (malloc_tiny_small(&g_malloc_zones.small, ZONE_SMALL, size));
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
	void 			*(*malloc_size[3])(size_t) = \
		{&malloc_tiny, &malloc_small, &malloc_large};
	void			*ptr;

	if (!size || size > MALLOC_MAX)
		return (NULL);
	pthread_mutex_lock(&g_malloc_mutex);

	ptr = malloc_size[MALLOC_SIZE(size)](size);
	MALLOC_ULTRA_VERBOSE(" %s[%p]%s\n", "\e[33m", ptr, "\e[0m");
	pthread_mutex_unlock(&g_malloc_mutex);


//	TESTING
	size_t i = 0;
	while (i < size)
	{
		((char*)ptr)[i] = 42;
		i++;
	}
	ft_printf("VALID!\n");//
//	TESTING
	return (ptr);
}
