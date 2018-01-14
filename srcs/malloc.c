/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 22:59:26 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/15 00:01:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "malloc.h"

void			*malloc(size_t size)
{
	(void)size;
	ft_printf("hello! from malloc\n");
	return (NULL);
}
