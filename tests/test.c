/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 05:01:34 by agrumbac          #+#    #+#             */
/*   Updated: 2018/02/13 16:23:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"
// # include <sys/mman.h>
// # include <pthread.h>
// # include <sys/resource.h>
// # include <errno.h>

int      main()
{
    struct rlimit	rlp;

    if (getrlimit(RLIMIT_DATA, &rlp))
    {
        ft_printf("fail\n");
        // return (0);
    }
        ft_printf("{%llu}\n", rlp.rlim_cur);
    // return (rlp.rlim_cur);
    // char *x = malloc(-1);
    // free(x);
    return (0);
}

//
// int			main(int ac, char **av)
// {
// 	int		i = 0;
// 	char	*ptr;
//
// 	// ptr[0] = 42;
// 	while (i < 1024)
// 	{
// 		// ptr = malloc(654);
// 		// malloc(400);
// 		// if (!ptr)
// 		// 	ft_printf("NULL\n");
// 		// ft_printf("A");
// 		// ptr[41] = 42;
// 		// ft_printf("B");
// 		i++;
// 	}
// 	// while (i < 1024)
// 	// {
// 	// 	ptr = (char *)malloc(8169);
// 	// 	ft_printf("{[%d]=", i);
// 	// 	int w = 0;
// 	// 	while (w < 8169)
// 	// 	{
// 	// 		ft_printf("%d.", w);
// 	// 		ptr[w] = 42;
// 	// 		w++;
// 	// 	}
// 	// 	ft_printf("}");
// 	// 	i++;
// 	// }
// 	// show_alloc_mem();
// 	return (0);
// }

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
