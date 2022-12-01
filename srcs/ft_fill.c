/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 11:59:42 by aarrien-          #+#    #+#             */
/*   Updated: 2022/12/01 18:41:04 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_save_paths(char **envp, t_pipex *gen)
{
	int		i;
	char	*tmp;
	char	*path;
	char	**split;

	i = 0;
	while (ft_strncmp(*envp, "PATH", 4) != 0)
		envp++;
	path = ft_strtrim(*envp, "PATH=");
	split = ft_split(path, ':');
	free(path);
	while (split[i])
	{
		tmp = split[i];
		split[i] = ft_strjoin(split[i], "/");
		free(tmp);
		i++;
	}
	gen->paths = split;
}

char	*ft_cmd_path(char *cmd, t_pipex *gen)
{
	int		i;
	char	*tmp;
	char	**s_cmd;

	i = 0;
	s_cmd = ft_split(cmd, ' ');
	while (gen->paths[i])
	{
		tmp = ft_strjoin(gen->paths[i], s_cmd[0]);
		if (access(tmp, F_OK | X_OK) == 0)
			break ;
		free(tmp);
		tmp = 0;
		i++;
	}
	ft_free_split(s_cmd);
	return (tmp);
}

void	ft_fill(char **input, t_pipex *gen, int n, char **envp)
{
	int	i;

	i = 0;
	ft_save_paths(envp, gen);
	gen->path_cmd = malloc((n + 1) * sizeof(char *));
	gen->cmds = malloc((n + 1) * sizeof(char **));
	while (n - i > 0)
	{
		//printf("i = %d y n = %d\n", i, n);
		gen->path_cmd[i] = ft_cmd_path(input[i], gen);
		gen->cmds[i] = ft_split(input[i], ' ');
		i++;
	}
	gen->path_cmd[i] = NULL;
	gen->cmds[i] = NULL;
	gen->npipes = n - 1;
	i = 0;
	gen->fds = malloc(gen->npipes * sizeof(int *));
	while (gen->npipes - i > 0)
	{
		gen->fds[i] = malloc(2 * sizeof(int));
		pipe(gen->fds[i]);
		i++;
	}
}