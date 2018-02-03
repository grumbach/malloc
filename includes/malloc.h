/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 19:59:11 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/03 09:28:43 by agrumbac         ###   ########.fr       */
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

# define MALLOC_PS		(4096)

# define MALLOC_PAGE(x)	((!!(x & 0xfff)) * MALLOC_PS)

# define MALLOC_SIZE(x)	(((!!((x) & 0xfffffffffffffC00)) << 1) | \
						(!!((x) & 0b1110000000) << (!!((x) & 0xfffffffffffffC00))))

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

void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);

void					show_alloc_mem();
void					show_alloc_mem_hex(void *ptr);

#endif
