/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmichaud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:08:06 by nmichaud          #+#    #+#             */
/*   Updated: 2026/02/16 16:08:07 by nmichaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*cmd_slash(char *cmd, int *exit_code)
{
	if (access(cmd, F_OK) == -1)
		*exit_code = 127;
	else if (access(cmd, X_OK) == -1)
		*exit_code = 126;
	else
		return (ft_strdup(cmd));
	return (NULL);
}

char	*get_path_line(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		else
			i++;
	}
	return (NULL);
}

static char	*get_full_path(char *cmd, char **path, int *exit_code, int *flag)
{
	char	*tmp;
	char	*full_path;
	int		i;

	(void)exit_code;
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			return (full_path);
		else if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == -1)
			*flag = 1;
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_cmd(char *cmd, char **path, int *exit_code)
{
	int		i;
	char	*full_path;
	int		flag;

	flag = 0;
	if (!cmd || cmd[0] == '\0')
	{
		*exit_code = 126;
		return (NULL);
	}
	if (ft_strchr(cmd, '/') != NULL)
		return (cmd_slash(cmd, exit_code));
	i = 0;
	full_path = get_full_path(cmd, path, exit_code, &flag);
	if (full_path != NULL)
		return (full_path);
	if (flag == 1)
		*exit_code = 126;
	else
		*exit_code = 127;
	return (NULL);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	/*char	**cmd_args;
	char	**paths;
	char	*full_path;
	int		exit_code;
	pid_t	pid;*/
	t_token	*token;

	(void)ac;
	(void)av;
	(void)env;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		/*pid = fork();
		if (pid == 0)
		{
			cmd_args = ft_split(line, ' ');
			paths = ft_split(get_path_line(env), ':');
			full_path = find_cmd(cmd_args[0], paths, &exit_code);
			if (!full_path)
				perror(NULL);
			execve(full_path, cmd_args, env);
		}
		wait(&pid);*/
		token = NULL;
		lexing(&token, line);
		t_token *tmp = token;
		while (tmp)
		{
			printf("%s\n", tmp->str);
			printf("%u\n", tmp->type);
			tmp = tmp->next;
		}
		free_token(&token);
		free(line);
	}
	return (0);
}
