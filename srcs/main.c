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


/* ************************************************************************** */
/*                            BUILTINS                                        */
/* ************************************************************************** */
typedef int (*t_builtin)(char *);

int exit_(char *input)
{
	(void)input;
	return (-1);
}

int echo_(char *input)
{
	ft_printf("%s\n", input + ft_strlen("echo "));
	return (0);
}

/* ************************************************************************** */
/*                            BINARIES                                        */
/* ************************************************************************** */

extern char **environ;

static char *get_env_variable(char *name)
{
	size_t i;
	size_t len;
	char *str;

	i = 0;
	len = ft_strlen(name);
	while ((str = environ[i]))
	{
		if (ft_strncmp(name, str, len) == 0 && str[len] == '=')
			return (str + len + 1);
			i++;
	}
	return (NULL);
}

static char *find_binary_path(char *name)
{
	char **split;
	char *res;
	size_t i;

	split = ft_strsplit(get_env_variable("PATH"), ':');
	i = 0;
	while (split[i])
	{
		if (!(res = ft_strnew(ft_strlen(split[i]) + ft_strlen(name) + 2)))
			return (NULL);
		ft_strcpy(res, split[i]);
		res[ft_strlen(split[i])] = '/';
		ft_strcpy(res + ft_strlen(split[i]) + 1, name);
		if (access(res, X_OK) == 0)
		{
			ft_free_strsplit(&split);
			return (res);
		}
		i++;
		free(res);
	}
	ft_free_strsplit(&split);
	ft_dprintf(STDERR_FILENO, "Could not find executable '%s'\n", name);
	return (NULL);
}

int		execute_binary(char *input)
{
	char **split;
	char *path;

	split = ft_strsplit(input, ' ');
	path = find_binary_path(split[0]);
	if (path)
	{
		execve(path, split + 1, NULL);
		free(path);
	}
	ft_free_strsplit(&split);
	return (0);
}

/* ************************************************************************** */
/*                            MAIN                                            */
/* ************************************************************************** */
int		handle_input(char *input)
{
	const char *commands[] = {"exit", "echo"};
	const t_builtin builtins[] = {&exit_, &echo_};
	size_t i;

	i = 0;
	while (i < sizeof(commands) / sizeof(char *))
	{
		if (!ft_strncmp(input, commands[i], ft_strlen(commands[i])))
			return (builtins[i](input));
		i++;
	}
	execute_binary(input);
	return (0);
}

int		main(void)
{
	char *input;
	int should_exit;

	input = NULL;
	should_exit = 0;
	while(!should_exit)
	{
		ft_putstr("$> ");
		if (gnlite(STDIN_FILENO, &input) < 0 || handle_input(input) < 0)
			should_exit = 1;
		if (input)
		{
			free(input);
			input = NULL;
		}
	}
	return (0);
}
