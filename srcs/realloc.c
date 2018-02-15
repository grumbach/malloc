/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:29 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/15 06:29:49 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline int	is_reallocable(t_malloc_chunk *chunk, size_t size)
{
	const int		malloc_size = MALLOC_SIZE(chunk->size);

	if (malloc_size == MALLOC_TINY)
		return (size <= ZONE_TINY);
	else if (malloc_size == MALLOC_SMALL)
		return (size <= ZONE_SMALL);
	else
		return (size + sizeof(t_malloc_chunk) <= \
			MALLOC_PAGE(chunk->size + sizeof(t_malloc_chunk)));
}

void				*realloc(void *ptr, size_t size)
{
	void			*new;

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("[realloc] of %s%lu%s at %s[%p]%s", "\e[33m", size, \
		"\e[0m", "\e[33m", ptr, "\e[0m");
	#endif

	if (!ptr || !size)
		return (malloc(size));
	pthread_mutex_lock(&g_malloc_mutex);
	if (malloc_out_of_zones(ptr))
	{
		#ifdef MALLOC_DEBUG_VERBOSE
		ft_printf("%s", malloc_out_of_zones(ptr) ? "\e[31m""[INVALID ADDR]""\e[0m" : "");
		#endif
		pthread_mutex_unlock(&g_malloc_mutex);
		return (NULL);
	}
	if (is_reallocable(ptr - sizeof(t_malloc_chunk), size))
	{
		#ifdef MALLOC_DEBUG_VERBOSE
		ft_printf("%s[SWEET]%s\n", "\e[35m", "\e[0m");
		#endif

		((t_malloc_chunk *)(ptr - sizeof(t_malloc_chunk)))->size = size;
		pthread_mutex_unlock(&g_malloc_mutex);
		return (ptr);
	}
	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("%s[BITTER]%s\n", "\e[35m", "\e[0m");
	#endif
	pthread_mutex_unlock(&g_malloc_mutex);
	new = malloc(size);
	if (!new)
		return (NULL);
	ft_memcpy(new, ptr, ((t_malloc_chunk *)(ptr - sizeof(t_malloc_chunk)))->size);
	free(ptr);
	return (new);
}

void				*reallocf(void *ptr, size_t size)
{
	void			*new;

	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	return (new);
}

void				*calloc(size_t count, size_t size)
{
	void			*ptr;

	ptr = malloc(count * size);
	if (ptr)
		ft_bzero(ptr, count * size);
	return (ptr);
}
