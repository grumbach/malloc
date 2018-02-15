/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/15 06:30:00 by agrumbac         ###   ########.fr       */
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
			MALLOC_ZONE * (malloc_size ? ZONE_SMALL : ZONE_TINY), "\e[0m");//
		#endif
		// unmap tiny small
		munmap(mem, MALLOC_ZONE * (malloc_size ? ZONE_SMALL : ZONE_TINY));
	}
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

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("%s[MUNMAP %p of %lu]%s", "\e[31m", chunk, msize, "\e[0m");//
	#endif

	// unmap large
	munmap(chunk, msize);
}

static void			free_chunk(t_malloc_chunk *chunk)
{
	const int			malloc_size = MALLOC_SIZE(chunk->size);
	void				*mem[2] = {g_malloc_zones.tiny, g_malloc_zones.small};

	if (malloc_size == MALLOC_LARGE)
		free_large(chunk);
	else
		free_tiny_small(chunk, malloc_size, mem[malloc_size]);
}

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

void				free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("%s[free]%s of %s[%p]%s", "\e[32m", "\e[0m", "\e[33m", ptr, "\e[0m");
	ft_printf("%s", malloc_out_of_zones(ptr) ? "\e[31m""[INVALID ADDR]""\e[0m" : "");
	#endif
	if (!(!ptr || malloc_out_of_zones(ptr)))
		free_chunk(ptr - sizeof(t_malloc_chunk));
	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("\n");
	#endif
	pthread_mutex_unlock(&g_malloc_mutex);
}
