/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 07:09:36 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/27 09:58:09 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
//
// static void	show_alloc_large()
// {
//
// }

void		show_alloc_mem(void)
{
	pthread_mutex_lock(&g_malloc_mutex);
	// show_alloc_large();
	pthread_mutex_unlock(&g_malloc_mutex);
}

void		show_alloc_mem_hex(void *ptr)
{
	(void)ptr;
	ft_printf("hello from show_alloc");
}

/*
** TINY : 0xA0000
** 0xA0020 - 0xA004A : 42 bytes
** 0xA006A - 0xA00BE : 84 bytes
** SMALL : 0xAD000
** 0xAD020 - 0xADEAD : 3725 bytes
** LARGE : 0xB0000
** 0xB0020 - 0xBBEEF : 48847 bytes
** Total : 52698 bytes
*/
