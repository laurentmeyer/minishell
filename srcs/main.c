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
/*                            LIBFT                                           */
/* ************************************************************************** */


static int ft_strfind(char* str, const char* array[], size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
	{
		if (ft_strequ(str, array[i]))
			return (i);
		i++;
	}
	return (-1);
}

size_t ft_strarray_length(char **src)
{
	size_t len;

	len = 0;
	while (src[len])
		len++;
	return len;
}

char **ft_strarray_new(size_t len)
{
	char **res;
	size_t i;

	if (!(res = (char **)malloc((len + 1) * sizeof(char *))))
		return (NULL);
	i = 0;
	while (i < len + 1)
		res[i++] = NULL;
	return (res);
}

int ft_strarray_realloc(char **src_ptr[], size_t increment)
{
	size_t len;
	size_t i;
	char **tmp;

	len = ft_strarray_length(*src_ptr);
	if (!(tmp = ft_strarray_new(len + increment)))
		return (-1);
	i = 0;
	while (i < len)
	{
		tmp[i] = src_ptr[0][i];
		i++;
	}
	free(*src_ptr);
	*src_ptr = tmp;
	return (0);
}

char **ft_strarray_cpy(char **src)
{
	size_t len;
	size_t i;
	char **res;

	len = ft_strarray_length(src);
	if (!(res = ft_strarray_new(len)))
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (!(res[i] = ft_strdup(src[i])))
		{
			ft_free_strsplit(&res);
			return (NULL);
		}
		i++;
	}
	return (res);
}


/* ************************************************************************** */
/*                            ENVIRONMENT                                     */
/* ************************************************************************** */

static int get_env_variable_index(char const *name, char *const envp[])
{
	int i;
	size_t len;
	const char *str;

	i = 0;
	len = ft_strlen(name);
	while ((str = envp[i]))
	{
		if (ft_strncmp(name, str, len) == 0 && str[len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char const *get_env_variable(char const *name, char *const envp[])
{
	int i;

	i = get_env_variable_index(name, envp);
	return (i < 0 ? NULL : envp[i] + ft_strlen(name) + 1);
}

/* ************************************************************************** */
/*                            EXPANSIONS                                      */
/* ************************************************************************** */

static int expand_env(char **str_ptr, char *envp[])
{
	char const *expansion;

	expansion = get_env_variable(*str_ptr + 1, envp);
	free(*str_ptr);
	*str_ptr = ft_strdup(expansion ? expansion : "");
	return (str_ptr ? 0 : -1);
}

static int expand_home(char **str_ptr, char *envp[])
{
	char const *expansion;
	char *tmp;
	size_t home_len;
	size_t expansion_len;

	home_len = ft_strlen(*str_ptr);
	expansion = get_env_variable("HOME", envp);
	expansion_len = ft_strlen(expansion);
	if (!(tmp = ft_strnew(expansion_len + home_len - 1)))
		return (-1); // error message
	ft_strcpy(tmp, expansion);
	ft_strcpy(tmp + expansion_len, *str_ptr + 1);
	free(*str_ptr);
	*str_ptr = tmp;
	return (0);
}

int expand_args(char **argv, char *envp[])
{
	size_t i;

	i = 0;
	while(argv[i])
	{
		if (ft_strequ(argv[i], "~") || ft_strnequ(argv[i], "~/", 2))
			expand_home(&argv[i], envp);
		if (argv[i][0] == '$')
			expand_env(&argv[i], envp);
		i++;
	}
	return (0); // handle errors
}

/* ************************************************************************** */
/*                            BUILTINS                                        */
/* ************************************************************************** */
typedef int (*t_builtin)(char **, char ***);

int exit_(char **argv, char **envp[])
{
	(void)argv;
	(void)envp;
	return (-1);
}

int echo_(char **argv, char **envp[])
{
	size_t i;
	
	(void)envp;
	i = 1;
	while (argv[i])
		ft_putendl(argv[i++]); // TODO améliorer 
	return (0);
}

int setenv_(char **argv, char ** envp[])
{
	char *tmp;
	int index;
	size_t name_len;

	if (!argv || !argv[0] || !argv[1] || !argv[2] || argv[3])
		return (-1); // FIXME: add error management
	name_len = ft_strlen(argv[1]);
	tmp = ft_strnew(name_len + 1 + ft_strlen(argv[2]));
	ft_strncpy(tmp, argv[1], name_len);
	tmp[name_len] = '=';
	ft_strcpy(tmp + name_len + 1, argv[2]);
	if ((index = get_env_variable_index(argv[1], *envp)) >= 0)
	{
		free(envp[0][index]);
		envp[0][index] = tmp;
	}
	else if (ft_strarray_realloc(envp, 1) == 0)
		envp[0][ft_strarray_length(*envp)] = tmp;
	else
		return (-1);
	return (0);
}

// int cd_(char **argv)
// {
// 	size_t i;
// 	char *
	
// 	if (!argv[0])
// 		return (-1);
// 	if 
// 	if(!argv[1])
// 		setenv_();
// 	i = 0;
// 	while (argv[i])
// 		ft_putendl(argv[i++]); // TODO améliorer 
// 	return (0);
// }

/* ************************************************************************** */
/*                            BINARIES                                        */
/* ************************************************************************** */

#include <sys/wait.h>

static char *find_binary_path(char *name, char *envp[])
{
	char **split;
	char *res;
	size_t i;

	split = ft_strsplit(get_env_variable("PATH", envp), ':');
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

int		execute_binary(char **argv, char *envp[])
{
	char *path;
	pid_t pid;
	int stat_loc;

	path = find_binary_path(argv[0], envp);
	if (path)
	{
		pid = fork();
		if (pid == 0)
			execve(path, argv, envp);
		else
		{
			waitpid(pid, &stat_loc, 0);
			free(path);
		}		
	}
	return (0);
}

/* ************************************************************************** */
/*                            MAIN                                            */
/* ************************************************************************** */

int		handle_input(char *input, char **envp[])
{
	const char *commands[] = {"exit", "echo", "setenv"}; //FIXME add cd
	const t_builtin builtins[] = {&exit_, &echo_, &setenv_};
	char **argv;
	int i;

	if (!(argv = ft_strsplit(input, ' ')))
		return (-1);
	if (expand_args(argv, *envp) < 0)
		return (-1);// output error message
	i = ft_strfind(argv[0], commands, sizeof(commands) / sizeof(char *)); // checker existence de argv[0] ?
	i = i < 0 ? execute_binary(argv, *envp) : builtins[i](argv, envp);
	ft_free_strsplit(&argv);
	return (i);
}

int		main(int argc, char **argv, char** env)
{
	char *input;
	int should_exit;
	char **envcpy;

	(void)argc;
	(void)argv;
	if (!(envcpy = ft_strarray_cpy(env)))
		return (-1); // error message
	input = NULL;
	should_exit = 0;
	while(!should_exit)
	{
		ft_putstr("$> ");
		if (gnlite(STDIN_FILENO, &input) < 0 || handle_input(input, &envcpy) < 0)
			should_exit = 1;
		if (input)
		{
			free(input);
			input = NULL;
		}
	}
	ft_free_strsplit(&envcpy);
	return (0);
}
