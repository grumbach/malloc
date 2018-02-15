/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 19:59:11 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/15 07:37:08 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include "libft.h"
# include <sys/mman.h>
# include <pthread.h>
# include <stddef.h>
# include <limits.h>

/*
** MALLOC_DEBUG flags
*/

# define MALLOC_DEBUG_VERBOSE

# ifndef MALLOC_DEBUG_VERBOSE
#  define MALLOC_ULTRA_VERBOSE(...)
# endif

# ifdef MALLOC_DEBUG_VERBOSE
#  define MALLOC_ULTRA_VERBOSE(...)	ft_printf(__VA_ARGS__)
# endif

/*
** ***************************** malloc private ********************************
** LARGE :
**     chained list of t_malloc_chunk
**               ______________       ______________
**     NULL <-> |t_malloc_chunk| <-> |t_malloc_chunk| <-> ... <-> NULL
** SMALL and TINY :
**     chained list of t_malloc_mem
**     each t_malloc_mem contains
**         - a list of free t_malloc_chunk
**         - a list of alloced t_malloc_chunk
**               ____________
**     NULL <-> |t_malloc_mem| <->  ... <-> NULL
**                |             ______________       ______________
**                |-> NULL <-> |t_malloc_chunk| <-> |t_malloc_chunk| <-> ...
**                |             ______________       ______________
**                |-> NULL <-> |t_malloc_chunk| <-> |t_malloc_chunk| <-> ...
*/

/*
** mapped_size == ZONE_(size) * MALLOC_ZONE
** chunk_size  == ZONE_(size) + sizeof(t_malloc_chunk)
*/
# define ZONE_SMALL		(1024)
# define ZONE_TINY		(128)
# define MALLOC_ZONE	(128)

/*
** MALLOC_PAGE(size) returns new_size with
**     (new_size > size)
**     &&
**     (new_size % MALLOC_PAGE_S == 0)
*/
# define PAGE_S			(4096)
# define MALLOC_PAGE(x)	((x) + (!!((x) % PAGE_S) * (PAGE_S - (x) % PAGE_S)))

/*
** MALLOC_SIZE(size) returns
**     (0) if size is TINY
**     (1) if size is SMALL
**     (2) if size is LARGE
*/
# define MALLOC_TINY	0
# define MALLOC_SMALL	1
# define MALLOC_LARGE	2
# define MALLOC_SIZE(x)	(((x) > ZONE_TINY) + ((x) > ZONE_SMALL))

/*
** MALLOC_MAX
** is the default max malloc size (in case getrlimit fails)
*/
# define MALLOC_MAX		UINT_MAX

typedef struct				s_malloc_chunk
{
	struct s_malloc_chunk	*next;
	struct s_malloc_chunk	*prev;
	size_t					size;
}							t_malloc_chunk;

typedef struct				s_malloc_mem
{
	struct s_malloc_mem		*next;
	struct s_malloc_mem		*prev;
	t_malloc_chunk			*alloc;
	t_malloc_chunk			*free;
}							t_malloc_mem;

typedef struct				s_malloc_zones
{
	t_malloc_mem			*tiny;
	t_malloc_mem			*small;
	t_malloc_chunk			*large;
}							t_malloc_zones;

extern t_malloc_zones		g_malloc_zones;
extern pthread_mutex_t		g_malloc_mutex;

int							malloc_out_of_zones(const void *ptr);

/*
** ***************************** malloc public *********************************
*/

void						free(void *ptr);
void						*malloc(size_t size);
void						*calloc(size_t count, size_t size);
void						*realloc(void *ptr, size_t size);
void						*reallocf(void *ptr, size_t size);

void						show_alloc_mem();
void						show_alloc_mem_hex(void *ptr);

#endif
