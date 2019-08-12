/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeyer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 11:52:44 by lmeyer            #+#    #+#             */
/*   Updated: 2019/08/12 11:53:05 by lmeyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

// #define MAXLEN 12

// void 		flush_(int fildes)
// {
// 	ssize_t read_res;
// 	char c;

// 	while ((read_res = read(fildes, &c, 1)) && c != '\n')
// 	{
// 		if (read_res < 0)
// 		{
// 			ft_dprintf(STDERR_FILENO, "read error\n");
// 			exit(0);
// 		}
// 	}
// }

// ssize_t read_(int fildes, void *buf, size_t nbyte)
// {
// 	ssize_t	i;

// 	if ((i = read(fildes, buf, nbyte)) < 0)
// 	{
// 		ft_dprintf(STDERR_FILENO, "read error\n");
// 		exit(0);
// 	}
// 	if (ft_strlen(buf) == MAXLEN)
// 	{
// 		flush_(fildes);
// 		ft_dprintf(STDERR_FILENO, "instruction is too long\n");
// 		exit(0);
// 	}
// 	return (i);
// }

// ssize_t write_(int fildes, const void *buf, size_t nbyte)
// {
// 	ssize_t	i;

// 	if ((i = write(fildes, buf, nbyte)) < 0)
// 		exit(0);
// 	return (i);
// }

int		handle_input(char *input)
{
	if (!ft_strcmp("exit", input))
		return (-1);
	ft_printf("%s\n", input);
	return (0);
}

int		main(void)
{
	char *input;
	int gnl;
	int should_exit;

	input = NULL;
	should_exit = 0;
	while(!should_exit)
	{
		write(STDOUT_FILENO, "$> ", 3);
		gnl = gnlite(STDIN_FILENO, &input);
		if (gnl < 0 || handle_input(input) < 0)
			should_exit = 1;
		if (input)
		{
			free(input);
			input = NULL;
		}
	}
	return (0);
}
