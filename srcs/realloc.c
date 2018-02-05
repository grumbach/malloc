/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:29 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/05 13:36:31 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void			*realloc(void *ptr, size_t size)
{
	pthread_mutex_lock(&g_malloc_mutex);
	ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED)
		ptr = NULL;
	ft_printf("[realloc] of %s%lu%s at %s[%p]%s\n", "\e[33m", size, "\e[0m", "\e[34m", ptr, "\e[0m");//
	pthread_mutex_unlock(&g_malloc_mutex);
	return (ptr);
}
