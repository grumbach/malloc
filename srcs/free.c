/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/23 03:42:01 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void			free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);
	(void)ptr;
	ft_printf("hello! from freeed\n");
	pthread_mutex_unlock(&g_malloc_mutex);
}
