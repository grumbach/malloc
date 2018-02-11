/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 19:59:11 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/11 23:15:16 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include "libft.h"
# include <sys/mman.h>
# include <pthread.h>
# include <stddef.h>

/*
** malloc private
**
** (2) LARGE :
** chained list of t_malloc_chunk
**           _______________       _______________
** NULL <-> |t_malloc_chunk| <-> |t_malloc_chunk| <-> ... <-> NULL
**
** (1) SMALL and (0) TINY :
** chained list of t_malloc_mem
** each t_malloc_mem contains
**     - a list of free t_malloc_chunk
**     - a list of alloced t_malloc_chunk
**           ____________
** NULL <-> |t_malloc_mem| <->  ... <-> NULL
**            |             _______________       _______________
**            |-> NULL <-> |t_malloc_chunk| <-> |t_malloc_chunk| <-> ...
**            |             _______________       _______________
**            |-> NULL <-> |t_malloc_chunk| <-> |t_malloc_chunk| <-> ...
**
*/

/*
** mapped_size == ZONE_(size) * MALLOC_ZONE
** chunk_size  == ZONE_(size) + sizeof(t_malloc_chunk)
*/
# define ZONE_SMALL		(4096)
# define ZONE_TINY		(1024)
# define MALLOC_ZONE	(128)

/*
** MALLOC_PAGE(size) returns new_size with
**     (new_size > size)
**     &&
**     (new_size % MALLOC_PAGE_S == 0)
*/
# define MALLOC_PAGE_S	(4096)
# define MALLOC_PAGE(x)	((x) + (!!((x) % MALLOC_PAGE_S) * \
						(MALLOC_PAGE_S - (x) % MALLOC_PAGE_S)))

/*
** MALLOC_SIZE(size) returns
**     (0) if size is TINY
**     (1) if size is SMALL
**     (2) if size is LARGE
*/
# define MALLOC_SIZE(x)	(((x) > ZONE_TINY) + ((x) > ZONE_SMALL))

typedef struct			s_malloc_chunk	t_malloc_chunk;
typedef struct			s_malloc_mem	t_malloc_mem;

typedef struct			s_malloc_chunk
{
	t_malloc_chunk		*next;
	t_malloc_chunk		*prev;
	size_t				size;
}						t_malloc_chunk;

typedef struct			s_malloc_mem
{
	t_malloc_mem		*next;
	t_malloc_mem		*prev;
	t_malloc_chunk		*alloc;
	t_malloc_chunk		*free;
}						t_malloc_mem;

typedef struct			s_malloc_zones
{
	t_malloc_mem		*tiny;
	t_malloc_mem		*small;
	t_malloc_chunk		*large;
}						t_malloc_zones;

extern t_malloc_zones	g_malloc_zones;
extern pthread_mutex_t	g_malloc_mutex;

/*
** malloc public
*/

# define MALLOC_DEBUG_VERBOSE

void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);

void					show_alloc_mem();
void					show_alloc_mem_hex(void *ptr);

#endif
