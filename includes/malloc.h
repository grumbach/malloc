/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 19:59:11 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/27 09:09:13 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include "libft.h"
# include <sys/mman.h>
# include <pthread.h>

/*
** malloc private
*/

# define MALLOC_MAX_TINY		128
# define MALLOC_MAX_SMALL		1024

typedef struct			s_malloc_chunck
{
	size_t				size;
	void				*next;
}						t_malloc_chunck;

typedef struct			s_malloc_mem
{
	t_malloc_chunck		*alloc;
	t_malloc_chunck		*free;
}						t_malloc_mem;

typedef struct			s_malloc_zones
{
	struct s_malloc_mem	tiny;
	struct s_malloc_mem	small;
	t_malloc_chunck		*large;
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
