#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

void	ft_putstr_error(char *msg, char *arg)
{
	while (*msg)
		write(2, msg++, 1);
	if (arg)
		while (*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);		
}

void	ft_execute(char **argv, int i, int tmp_fd, char **env)
{
	argv[i] = NULL;
	dup2(tmp_fd, 0);
	close(tmp_fd);
	execve(argv[0], argv, env);
	ft_putstr_error("error: cannot execute", NULL);
	exit(1);
}

int	main(int argc, char **argv, char **env)
{
	int		i = 0;
	int		fd[2];
	int		tmp_fd = dup(0);
	pid_t	pid;

	(void)argc;
	while (argv[i] && argv[i + 1])
	{
		argv = &argv[i + 1];
		i = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (strcmp(argv[0], "cd") == 0)
		{
			if (i != 2)
				ft_putstr_error("error: cd: bad arguments", NULL);
			else if (chdir(argv[1]) != 0)
				ft_putstr_error("error: cd: cannot change directory to ", argv[1]);
		}
		else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0))
		{
			pid = fork();
			if (pid == 0)
				ft_execute(argv, i, tmp_fd, env);
			else if (pid > 0)
			{
				close(tmp_fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1);
				tmp_fd = dup(0);
			}
			else
				ft_putstr_error("error: fatal", NULL);
		}
		else if (i != 0 && strcmp(argv[i], "|") == 0)
		{
			if (pipe(fd) == -1)
				ft_putstr_error("error: fatal", NULL);
			pid = fork();
			if (pid == 0)
			{
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
				ft_execute(argv, i, tmp_fd, env);
			}
			else if (pid > 0)
			{
				close(fd[1]);
				close(tmp_fd);
				tmp_fd = fd[0];
			}
			else
				ft_putstr_error("error: fatal", NULL);
		}
	}
	close(tmp_fd);
	return (0);
}
