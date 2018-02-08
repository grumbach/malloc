/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:29 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/08 07:37:21 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void			*realloc(void *ptr, size_t size)
{
	pthread_mutex_lock(&g_malloc_mutex);
	ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED)
		ptr = NULL;

	#ifdef MALLOC_DEBUG_VERBOSE
	ft_printf("[realloc] of %s%lu%s at %s[%p]%s\n", "\e[33m", size, "\e[0m", "\e[33m", ptr, "\e[0m");//
	#endif

	pthread_mutex_unlock(&g_malloc_mutex);
	return (ptr);
}

void			*reallocf(void *ptr, size_t size)
{
	void		*new;

	new = realloc(ptr, size);
	if (new != ptr)
		free(ptr);
	return (new);
}

void			*calloc(size_t count, size_t size)
{
	void		*ptr;

	ptr = malloc(count * size);
	if (ptr)
		ft_bzero(ptr, count * size);
	return (ptr);
}
