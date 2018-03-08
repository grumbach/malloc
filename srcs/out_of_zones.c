/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_of_zones.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 20:27:33 by agrumbac          #+#    #+#             */
/*   Updated: 2018/03/08 20:27:56 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

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
