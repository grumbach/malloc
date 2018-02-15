/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:29 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/15 07:50:37 by agrumbac         ###   ########.fr       */
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

static inline void	*bitter_realloc(void *ptr, size_t size)
{
	void			*new;
	size_t			old_size;

	MALLOC_ULTRA_VERBOSE("%s[BITTER]%s\n", "\e[35m", "\e[0m");
	new = malloc(size);
	if (!new)
		return (NULL);
	old_size = ((t_malloc_chunk *)(ptr - sizeof(t_malloc_chunk)))->size;
	ft_memcpy(new, ptr, old_size);
	free(ptr);
	return (new);
}

void				*realloc(void *ptr, size_t size)
{
	MALLOC_ULTRA_VERBOSE("[realloc] of %s%lu%s at %s[%p]%s", "\e[33m", size, \
		"\e[0m", "\e[33m", ptr, "\e[0m");
	if (!ptr || !size)
		return (malloc(size));
	pthread_mutex_lock(&g_malloc_mutex);
	if (malloc_out_of_zones(ptr))
	{
		MALLOC_ULTRA_VERBOSE("\e[31m""[INVALID ADDR]""\e[0m");
		pthread_mutex_unlock(&g_malloc_mutex);
		return (NULL);
	}
	if (is_reallocable(ptr - sizeof(t_malloc_chunk), size))
	{
		MALLOC_ULTRA_VERBOSE("%s[SWEET]%s\n", "\e[35m", "\e[0m");
		((t_malloc_chunk *)(ptr - sizeof(t_malloc_chunk)))->size = size;
		pthread_mutex_unlock(&g_malloc_mutex);
		return (ptr);
	}
	pthread_mutex_unlock(&g_malloc_mutex);
	return (bitter_realloc(ptr, size));
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
