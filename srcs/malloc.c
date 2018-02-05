/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:26 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/05 16:57:56 by agrumbac         ###   ########.fr       */
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

static inline void	mem_init_zone(t_malloc_mem *mem, \
					t_malloc_mem **malloc_zone, const size_t zone_size)
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
	// while there is room for a chunk (t_malloc_chunk + zone_size) in the mem
	while ((void*)free_chunk + sizeof(t_malloc_chunk) + zone_size < \
		(void*)(zone_size * MALLOC_ZONE))
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

static inline void	*malloc_tiny_small(t_malloc_mem **source, \
					const size_t zone_size, const size_t size)
{
	t_malloc_mem	*mem;

	//check for mem
	mem = *source;
	ft_printf("{%p}", g_malloc_zones.tiny ? g_malloc_zones.tiny->free : (void*)42);//print all data struct in show_alloc_mem
	ft_printf("[");//
	t_malloc_mem *DEBUG_test = mem;//
	while (mem && !mem->free)
		mem = mem->next;
	ft_printf("free mem %s%p%s] ", mem == DEBUG_test ? "\e[34m" : "", mem, "\e[0m");//
	//if no memory or no free memory
	if (!mem)
	{
		mem = mmap(0, zone_size * MALLOC_ZONE, \
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		ft_printf("%s[MMAP %p]%s", "\e[31m", mem, "\e[0m");//
		if (mem == MAP_FAILED)
			return (NULL);
		mem_init_zone(mem, source, zone_size);
	}
	//mem available t_malloc_mem
	return (give_chunk(&mem->free, &mem->alloc, size));
}

static void			*malloc_tiny(size_t size)
{
	ft_printf("[tiny] of %s%lu%s\t", "\e[33m", size, "\e[0m");//
	return (malloc_tiny_small(&g_malloc_zones.tiny, 128, size));
}

static void			*malloc_small(size_t size)
{
	ft_printf("[small] of %s%lu%s\t", "\e[33m", size, "\e[0m");//
	return (malloc_tiny_small(&g_malloc_zones.small, 1024, size));
}

static void			*malloc_large(size_t size)
{
	t_malloc_chunk	*ptr;

	ft_printf("[large] of %s%lu%s\t", "\e[33m", size, "\e[0m");//
	ptr = mmap(0, size + sizeof(t_malloc_chunk) + \
		MALLOC_PAGE(size + sizeof(t_malloc_chunk)), \
		PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	ft_printf("%s[MMAP %p]%s", "\e[31m", ptr, "\e[0m");//
	if (ptr == MAP_FAILED)
		return (NULL);
	*ptr = (t_malloc_chunk){g_malloc_zones.large, NULL, size};
	if (g_malloc_zones.large)
		g_malloc_zones.large->prev = ptr;
	g_malloc_zones.large = ptr;
	return (ptr + 1);//check this [] + ptr arythm
}

/*
**	malloc : 3 cases
**	108 zones of 127- (+24) per TINY page (76 bytes wasted)
**	125 zones of 1023- (+24) per SMALL page (197 bytes wasted)
**
** "TINY" mallocs, from 1 to 127 bytes, will be stored in N bytes big zones.
** "SMALL" mallocs, from 128 to 1023 bytes, will be stored in M bytes big zones.
** "LARGE" mallocs, from 1024 bytes and more, will be stored out of zone,
**    which simply means with mmap(), they will be in a zone on their own.
**	0b 111 11xx xxxx xxxx ==> large 2         & ... 1111 1100 0000 0000
**	0b 000 0011 1xxx xxxx ==> small 1         &     0000 0011 1000 0000
**	0b 000 0000 0xxx xxxx ==> tiny  0
*/

void				*malloc(size_t size)
{
	void 			*(*malloc_size[3])(size_t) = \
		{&malloc_tiny, &malloc_small, &malloc_large};
	void			*ptr;

	if (!size)
		return (NULL);
	pthread_mutex_lock(&g_malloc_mutex);

	ptr = malloc_size[MALLOC_SIZE(size)](size);

	ft_printf(" %s[%p]%s\n", "\e[33m", ptr, "\e[0m");//

	pthread_mutex_unlock(&g_malloc_mutex);
	return (ptr);
}
