/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:29 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/15 00:01:42 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "malloc.h"

void			*realloc(void *ptr, size_t size)
{
	(void)ptr;
	(void)size;
	ft_printf("hello! from realloc\n");
	return (NULL);
}
