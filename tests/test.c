/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 05:01:34 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/03 03:30:03 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

# include <sys/mman.h>
# include <pthread.h>
# include <sys/resource.h>
# include <errno.h>

int			main(int ac, char **av)
{
	void	*a = malloc(896);
	ft_printf("oh! %p\n", a);

	return (0);
}

// MUTEX TEST
// pthread_mutex_t		mut = PTHREAD_MUTEX_INITIALIZER;
//
// void		*tryhard(int **i)
// {
// 	int		err;
//
// 	if ((err = pthread_mutex_lock(&mut)))
// 		return ft_printf("ERROR %d\n", err);
// 	ft_printf("tryhard = %p\n", *i);
// 	*i = malloc(42);
// 	**i = 42;
// 	ft_printf("tryhard = %d\n", **i);
// 	ft_printf("tryhard = %p\n", *i);
// 	if ((err = pthread_mutex_unlock(&mut)))
// 		return ft_printf("ERROR %d\n", err);
// }
//
// int			main(void)
// {
// 	int			err;
// 	int			*i = NULL;
// 	pthread_t	t;
//
// 	pthread_create(&t, NULL, tryhard, &i);
//
// 	if ((err = pthread_mutex_lock(&mut)))
// 		return ft_printf("ERROR %d\n", err);
// 	ft_printf("main = %p\n", i);
// 	if (i)
// 		ft_printf("main = %d\n", *i);
// 	free(i);
// 	ft_printf("main = %p\n", i);
// 	if ((err = pthread_mutex_unlock(&mut)))
// 		return ft_printf("ERROR %d\n", err);
// 	pthread_join(t, NULL);
// 	ft_printf("end = %d\n", i);
// 	return (0);
// }

// MMMAP TEST
//
// # define SIZE		(4096)
// # define USED_SIZE	(SIZE)
//
// int			main(int ac, char **av)
// {
// 	char	*var;
// 	char	*var2;
// 	rlim_t	rlp;
//
// 	var = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
// 	var2 = mmap(var + 4096, SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
//
//
// 	ft_printf("dist %d\n", var - var2);
//
// 	if ((getrlimit(RLIMIT_DATA, &rlp)))
// 		return printf("rlim fail %s\n", strerror(errno));
// 	ft_printf("rlimit %lb\n", rlp);
//
//
// 	ft_memset(var, 42, USED_SIZE);
// 	ft_memset(var2, 42, USED_SIZE);
// 	printf("%.*s\n", USED_SIZE, var);
// 	ft_printf("pagesize = %d, rlimit\n", getpagesize());
// 	munmap(var, SIZE);
//
// 	printf("%.*s\n", USED_SIZE, var2);
// 	munmap(var2, SIZE);
// 	return (0);
// }
