/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:26 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/27 10:42:24 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_malloc_zones		g_malloc_zones = {{NULL, NULL}, {NULL, NULL}, NULL};
pthread_mutex_t		g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

static void	*malloc_tiny(size_t size)
{
	ft_printf("hi from tiny : %lu\n", size);
	return (void*)size;
}

static void	*malloc_small(size_t size)
{
	ft_printf("hi from small : %lu\n", size);
	return (void*)size;
}

static void	*malloc_large(size_t size)
{
	t_malloc_chunck		*ptr;

	ptr = mmap(0, size + sizeof(t_malloc_chunck), \
		PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED)
		return (NULL);
	*ptr = (t_malloc_chunck){size, g_malloc_zones.large};
	g_malloc_zones.large = ptr;

	ft_printf("hi from large : %lu\n", size);//

	return (ptr + sizeof(t_malloc_chunck));
}

/*
**	malloc : 3 cases
** "TINY" mallocs, from 1 to 127 - 16 bytes, will be stored in N bytes big zones.
** "SMALL" mallocs, from 128 to 1023 - 16 bytes, will be stored in M bytes big zones.
** "LARGE" mallocs, from 1024 - 16 bytes and more, will be stored out of zone,
**    which simply means with mmap(), they will be in a zone on their own.
**	0b 111 11xx xxxx xxxx ==> large 2         & ... 1111 1100 0000 0000
**	0b 000 0011 1xxx xxxx ==> small 1         &     0000 0011 1000 0000
**	0b 000 0000 0xxx xxxx ==> tiny  0
*/

void		*malloc(size_t size)
{
	static void *(*malloc_size[3])(size_t) = \
		{&malloc_tiny, &malloc_small, &malloc_large};
	void		*ptr;

	if (!size)
		return (NULL);
	pthread_mutex_lock(&g_malloc_mutex);

	ptr = malloc_size[2](size);//TODO right size bitwise

	pthread_mutex_unlock(&g_malloc_mutex);
	return (ptr);
}


// ft_printf("hello! from malloc %d\n",
// (((!!(size & 0xFFFFFFFFFFFFFC00)) << 1) | \
//      (!!(size & 0b1110000000) << (!!(size & 0xFFFFFFFFFFFFFC00)))));
//
// malloc_size[(((!!(size & 0xFFFFFFFFFFFFFC00)) << 1) | \
//      (!!(size & 0b1110000000) << (!!(size & 0xFFFFFFFFFFFFFC00))))](size);
